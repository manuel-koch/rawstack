#include "ufrawworker.h"
#include "ufrawprocess.h"
#include "ufrawconfig.h"
#include "enfuseprocess.h"

#include <vector>

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
    if( !cfg )
        return;

    double exposureShift = 1.5;
    if( idx == 0 )
        exposureShift = 0;
    else
        exposureShift *= (double)idx / (nof/2);
    double exposure = cfg->exposure() + exposureShift;

    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( cfg->raw() );
    ufraw.setExposure( exposure );
    ufraw.setWbTemperature( cfg->wbTemperature() );
    ufraw.setWbGreen( cfg->wbGreen() );

    if( idx < 0 )
        ufraw.setRestore( UfrawProcess::RestoreHsv );
    else if( idx > 0 )
        ufraw.setClip( UfrawProcess::ClipDigital );
    else
        ufraw.setClip( UfrawProcess::ClipFilm );

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
    if( !cfg || cfg->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::gmpreview() invalid config";
        return Magick::Image();
    }

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( cfg->raw() );

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
    if( !cfg || cfg->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::prepareImpl() invalid config";
        return;
    }

    if( cfg->wbTemperature() != UfrawConfig::DefaultWbTemperature &&
        cfg->wbGreen()       != UfrawConfig::DefaultWbGreen )
        return;

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( cfg->raw() );
    ufraw.setExposure( cfg->exposure() );
    ufraw.setShrink( 4 );

    // probe for other settings first...
    ufraw.run( UfrawProcess::OutputProbe );
    if( cfg->wbTemperature() == UfrawConfig::DefaultWbTemperature )
        cfg->setWbTemperature( ufraw.wbTemperature() );
    if( cfg->wbGreen() == UfrawConfig::DefaultWbGreen )
        cfg->setWbGreen( ufraw.wbGreen() );
}

void UfrawWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    qDebug() << "UfrawWorker::developImpl()" << this << predecessor;

    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg || cfg->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::developImpl() invalid config";
        return;
    }

    double progressPhaseA = 0.3;
    double progressPhaseB = 0.6;

    int nof = cfg->fuse();
    std::vector<Magick::Image> imgs(nof);
    std::vector<UfrawProcess>  ufraw(nof);
    int firstIdx = -(nof-1)/2;
    for( int i=0; i<nof; i++ )
    {
        setProgress( double(i)/nof*progressPhaseA );
        run( ufraw[i], preview, firstIdx+i, nof );
    }
    for( int i=0; i<nof; i++ )
    {
        setProgress( progressPhaseA+double(i)/nof*progressPhaseB );
        ufraw[i].waitForFinished(-1);
        qDebug() << "UfrawWorker::developImpl()" << i << "finished with exitcode" << ufraw[i].exitCode() << ":" << ufraw[i].console();
        if( ufraw[i].exitCode() == 0 )
        {
            qDebug() << "UfrawWorker::developImpl()" << i << "loading" << ufraw[i].output();
            imgs[i].read( ufraw[i].output().toStdString().c_str() );
        }
    }

    QStringList rawImages;
    for( int i=0; i<nof; i++ )
    {
        rawImages << ufraw[i].output();
        // FIXME: remove saving exposed images
        imgs[i].write( QString("/Users/manuel/tmp/test_%1_%2.tif").arg(cfg->name()).arg(i).toStdString() );
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
        m_img = imgs[0];
    }
}
