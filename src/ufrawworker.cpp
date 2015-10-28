#include "ufrawworker.h"
#include "ufrawprocess.h"
#include "ufrawconfig.h"
#include "commonconfig.h"
#include "enfuseprocess.h"

#include <QDebug>
#include <QThread>
#include <QTemporaryFile>

UfrawWorker::UfrawWorker()
    : WorkerBase()
{
    // EMPTY
}

UfrawWorker::~UfrawWorker()
{
    // EMPTY
}

// Argument idx is in range of -n ... n, e.g. -3 ... 3
// Argument nof is the total number of used indices, e.g. 7
void UfrawWorker::run( UfrawProcess &ufraw, bool preview, int idx, int nof )
{
    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg || !m_common )
        return;

    double exposureShift = 2;
    if( idx == 0 )
        exposureShift = 0;
    else
        exposureShift *= (double)idx / (nof/2);
    double exposure = cfg->exposure() + exposureShift;

    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( m_common->raw() );
    ufraw.setExposure( exposure );
    ufraw.setWbTemperature( cfg->wbTemperature() );
    ufraw.setWbGreen( cfg->wbGreen() );

    if( idx > 0 )
    {
        // over exposed
        ufraw.setClip( UfrawProcess::ClipDigital );
    }
    else
    {
        // normal / under exposed
        ufraw.setRestore( UfrawProcess::RestoreHsv );
        ufraw.setClip( UfrawProcess::ClipFilm );
    }

    if( preview )
    {
        ufraw.setInterpolate( UfrawProcess::InterpolateBilinear );
        ufraw.setColorSmoothing( false );
        ufraw.setShrink( 4 );
    }

    // extract the image
    ufraw.run( UfrawProcess::OutputImage );
}

const Magick::Image UfrawWorker::gmpreview()
{
    if( m_preview.isValid() )
        return m_preview;

    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg || !m_common || m_common->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::gmpreview() invalid config";
        return Magick::Image();
    }

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( m_common->raw() );

    // extract the thumbnail image
    ufraw.run( UfrawProcess::OutputThumbnail );
    m_preview.read( ufraw.output().toStdString().c_str() );
    m_preview.magick("JPEG");
    m_preview.quality(92);
    m_preview.matte(false);
    qDebug() << "UfrawWorker::gmpreview()" << m_preview.format().c_str();

    return m_preview;
}

void UfrawWorker::prepareImpl()
{
    qDebug() << "UfrawWorker::prepareImpl()" << this;

    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg  || !m_common || m_common->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::prepareImpl() invalid config";
        return;
    }

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( m_common->raw() );
    ufraw.setExposure( cfg->exposure() );
    ufraw.setShrink( 2 );

    // probe for other settings first...
    ufraw.run( UfrawProcess::OutputProbe );

    UfrawProcess::InfoMap info = ufraw.info();
    QVariantMap exif;
    for( auto it = info.begin(); it != info.end(); it++ )
    {
        exif[it.key()] = it.value();
    }
    m_common->setExif( exif );

    if( cfg->wbTemperature() == UfrawConfig::DefaultWbTemperature ||
        cfg->wbGreen()       == UfrawConfig::DefaultWbGreen          )
    {
        cfg->setWbTemperature( ufraw.wbTemperature() );
        cfg->setWbGreen( ufraw.wbGreen() );
    }
}

// returns a masked version of given image where every pixel
// above / below a given value ( in quantum range ) gets transparent
Magick::Image masked( bool below, double value, Magick::Image img )
{
    Magick::Image mask = img;
    mask.type( Magick::GrayscaleMatteType );
    mask.normalize();
    double v = below ? value : 1-value;
    mask.threshold( v*MaxRGB );
    if( below )
        mask.negate();
    mask.type( Magick::TrueColorMatteType );
    mask.transparent( "black" );

    Magick::Image masked = img;
    masked.composite( mask, Magick::CenterGravity, Magick::CopyOpacityCompositeOp );
    return masked;
}

void UfrawWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    qDebug() << "UfrawWorker::developImpl()" << this << predecessor;

    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg  || !m_common || m_common->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::developImpl() invalid config";
        return;
    }

    double progressPhaseA = 0.1;
    double progressPhaseB = 0.5;

    int nof = cfg->fuse();
    Magick::Image normalImg;
    std::vector<UfrawProcess>  ufraw(nof);
    int firstIdx = -(nof-1)/2;
    for( int i=0; i<nof; i++ )
    {
        setProgress( double(i) / nof * progressPhaseA );
        run( ufraw[i], preview, firstIdx+i, nof );
    }
    for( int i=0; i<nof; i++ )
    {
        setProgress( progressPhaseA + double(i) / nof * progressPhaseB );
        ufraw[i].waitForFinished(-1);
        qDebug() << "UfrawWorker::developImpl()" << i << "finished with exitcode" << ufraw[i].exitCode() << ":" << ufraw[i].console();
        if( ufraw[i].exitCode() == 0 )
        {
            bool isNormalExposed = (firstIdx+i)==0;
            bool isOverExposed   = (firstIdx+i)>0;
            qDebug() << "UfrawWorker::developImpl()" << i << "loading" << ufraw[i].output();
            Magick::Image img, mask;
            img.read( ufraw[i].output().toStdString().c_str() );
            if( isNormalExposed )
            {
                normalImg = img;
            }
            else
            {
                mask = masked( isOverExposed, 0.98, img );
                mask.write( ufraw[i].output().toStdString().c_str() );
            }
        }
    }

    QStringList rawImages;
    for( int i=0; i<nof; i++ )
    {
        rawImages << ufraw[i].output();
    }

    if( nof > 1 )
    {
        EnfuseProcess enfuse;
        enfuse.setProgram( "/opt/local/bin/enfuse" );
        enfuse.setInputs( rawImages );
        connect( &enfuse, &EnfuseProcess::progress, [&](double progress) {
            setProgress( progressPhaseA + progressPhaseB + (1-progressPhaseA-progressPhaseB)*progress );
        });
        enfuse.run();
        enfuse.waitForFinished(-1);
        qDebug() << "UfrawWorker::developImpl() enfuse finished with exitcode" << enfuse.exitCode() << ":" << enfuse.console();
        if( enfuse.exitCode() == 0 )
        {
            m_img.read( enfuse.output().toStdString().c_str() );
            m_img.matte(false);
            qDebug() << "UfrawWorker::developImpl() fused" << m_img.format().c_str();
        }
    }
    else
    {
        m_img = normalImg;
    }
}
