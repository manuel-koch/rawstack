#include "ufrawworker.h"
#include "ufrawprocess.h"
#include "ufrawsettings.h"
#include "configdbentry.h"
#include "configsetting.h"
#include "enfuseprocess.h"

#include "libraw/libraw.h"

#include <QDebug>
#include <QThread>
#include <QTemporaryFile>

UfrawWorker::UfrawWorker()
    : WorkerBase("ufraw")
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
    Q_ASSERT( config() );

    double exposureShift = 2;
    if( idx == 0 )
        exposureShift = 0;
    else
        exposureShift *= (double)idx / (nof/2);
    double exposure = config()->settings()->getSetting(UfrawSettings::Exposure)->value().toDouble() + exposureShift;

    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( config()->raw() );
    ufraw.setExposure( exposure );
    ufraw.setWbTemperature( config()->settings()->getSetting(UfrawSettings::WbTemperature)->value().toInt() );
    ufraw.setWbGreen( config()->settings()->getSetting(UfrawSettings::WbGreen)->value().toDouble()  );

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

void UfrawWorker::prepareImpl()
{
    qDebug() << "UfrawWorker::prepareImpl()" << this;

    if( config()->settings()->getSetting(UfrawSettings::WbTemperature)->value().toInt() == UfrawSettings::DefaultWbTemperature ||
        config()->settings()->getSetting(UfrawSettings::WbGreen)->value().toInt()       == UfrawSettings::DefaultWbGreen          )
    {
        // probe for resulting settings...
        UfrawProcess ufraw;
        ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
        ufraw.setRaw( config()->raw() );
        ufraw.setExposure( config()->settings()->getSetting(UfrawSettings::Exposure)->value().toDouble() );
        ufraw.setShrink( 8 );
        ufraw.run( UfrawProcess::OutputProbe );
        if( ufraw.failed() )
        {
            qWarning() << "UfrawWorker::prepareImpl() ufraw failed, returned" << ufraw.exitCode();
            return;
        }
        // use the resulting settings
        config()->settings()->getSetting(UfrawSettings::WbTemperature)->setValue( ufraw.wbTemperature() );
        config()->settings()->getSetting(UfrawSettings::WbGreen)->setValue( ufraw.wbGreen() );
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

void UfrawWorker::registerSettingsImpl()
{
    hashSetting(UfrawSettings::Fuse);
    hashSetting(UfrawSettings::ExposureSigma);
    hashSetting(UfrawSettings::Exposure);
    hashSetting(UfrawSettings::WbTemperature);
    hashSetting(UfrawSettings::WbGreen);
}

void UfrawWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    qDebug() << "UfrawWorker::developImpl()" << this << predecessor;

    double progressPhaseA = 0.1;
    double progressPhaseB = 0.5;

    int nof = config()->settings()->getSetting(UfrawSettings::Fuse)->value().toInt();
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
                normalImg.write( QString("/Users/manuel/tmp/test_normal.tif").toStdString().c_str() );
            }
            else
            {
                mask = masked( isOverExposed, 0.98, img );
                mask.write( ufraw[i].output().toStdString().c_str() );
                mask.write( QString("/Users/manuel/tmp/test_masked%1.tif").arg(i).toStdString().c_str() );
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
        double sigma = config()->settings()->getSetting(UfrawSettings::ExposureSigma)->value().toDouble();
        EnfuseProcess enfuse;
        enfuse.setProgram( "/opt/local/bin/enfuse" );
        enfuse.setInputs( rawImages );
        enfuse.setExposureSigma( sigma );
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
