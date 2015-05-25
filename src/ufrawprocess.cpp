#include <QDebug>

#include "ufrawprocess.h"

UfrawProcess::UfrawProcess(QObject *parent)
    : QProcess(parent)
    , m_shrink(1)
    , m_exposure(0)
    , m_interpolate(InterpolateAhd)
    , m_restore(RestoreClip)
    , m_clip(ClipDigital)
{
    connect( this, SIGNAL(finished(int)),                        this, SLOT(onTerminated(int)) );
    connect( this, SIGNAL(started()),                            this, SLOT(onStarted()) );
    connect( this, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onStateChanged(QProcess::ProcessState)) );
    connect( this, SIGNAL(error(QProcess::ProcessError)),        this, SLOT(onError(QProcess::ProcessError)) );
    connect( this, SIGNAL(readyReadStandardOutput()),            this, SLOT(onOutData()) );
    connect( this, SIGNAL(readyReadStandardError()),             this, SLOT(onErrData()) );
}

UfrawProcess::~UfrawProcess()
{
    if( state() != QProcess::Running )
        waitForFinished();
}

void UfrawProcess::run()
{
    if( state() == QProcess::Running )
        return;
    qDebug() << "UfrawProcess::run() starting...";

    QString interpolate;
    switch( m_interpolate )
    {
        case InterpolateBilinear:  interpolate = "bilinear";   break;
        case InterpolateFourColor: interpolate = "four-color"; break;
        case InterpolatePpg:       interpolate = "ppg";        break;
        case InterpolateVng:       interpolate = "vng";        break;
        case InterpolateAhd:       interpolate = "ahd";        break;
        default:                   interpolate = "bilinear";   break;
    }

    QString clip;
    switch( m_clip )
    {
        case ClipDigital: clip = "digital"; break;
        case ClipFilm:    clip = "film";    break;
        default:          clip = "film";    break;
    }

    QString restore;
    switch( m_restore )
    {
        case RestoreClip: restore = "clip"; break;
        case RestoreHsv:  restore = "hsv";  break;
        case RestoreLch:  restore = "lch";  break;
        default:          restore = "clip";   break;
    }

    QStringList args;
    args << "--overwrite" << "--zip"<< "--noexif"
         << "--out-depth=16" << "--out-type=ppm"
         << "--create-id=no" << "--output=-"
         << "--lensfun=auto"
         << "--auto-crop"
         << "--rotate=camera"
         << QString("--shrink=%1").arg(m_shrink)
         << QString("--exposure=%1").arg(m_exposure)
         << QString("--interpolation=%1").arg(interpolate)
         << QString("--restore=%1").arg(restore)
         << QString("--clip=%1").arg(clip)
         << "--wb=camera"
         << "--color-smoothing"
         << m_raw;
    qDebug() << "UfrawProcess::run()" << args;
    setArguments( args );
    start();
}

void UfrawProcess::setRaw(QString raw)
{
    m_raw = raw;
}

void UfrawProcess::setShrink(int shrink)
{
    switch( shrink )
    {
        case 1:
        case 2:
        case 4:
        case 8:
            m_shrink = shrink;
            break;
        default:
            m_shrink = 1;
            break;
    }
}

void UfrawProcess::setExposure(double exposure)
{
    m_exposure = exposure;
}

void UfrawProcess::setInterpolate(UfrawProcess::Interpolate interpolate)
{
    m_interpolate = interpolate;
}

void UfrawProcess::setRestore(UfrawProcess::Restore restore)
{
    m_restore = restore;
}

void UfrawProcess::setClip(UfrawProcess::Clip clip)
{
    m_clip = clip;
}

void UfrawProcess::onTerminated(int exitCode)
{
    qDebug() << "UfrawProcess::onTerminated()" << exitCode;
}

void UfrawProcess::onStarted()
{
    qDebug() << "UfrawProcess::onStarted() pid" << pid();
}

void UfrawProcess::onStateChanged(QProcess::ProcessState newState)
{
    qDebug() << "UfrawProcess::onStateChanged()" << newState;
}

void UfrawProcess::onError(QProcess::ProcessError error)
{
    qDebug() << "UfrawProcess::onError()" << error;
}

void UfrawProcess::onOutData()
{
    m_outData.append( readAllStandardOutput() );
    qDebug() << "UfrawProcess::onOutData()" << m_outData.size();
    emit progress();
}

void UfrawProcess::onErrData()
{
    qDebug() << "UfrawProcess::onErrData()";
    m_errData.append( readAllStandardError() );
    emit progress();
}
