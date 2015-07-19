#include "ufrawprocess.h"

#include <QDebug>
#include <QTemporaryFile>
#include <QRegularExpression>

UfrawProcess::UfrawProcess(QObject *parent)
    : QProcess(parent)
    , m_shrink(1)
    , m_exposure(0)
    , m_interpolate(InterpolateAhd)
    , m_restore(RestoreClip)
    , m_clip(ClipDigital)
    , m_wbTemperature(0)
    , m_wbGreen(0)
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

void UfrawProcess::run(bool probe)
{
    if( state() == QProcess::Running )
        return;
    qDebug() << "UfrawProcess::run()" << (probe ? "probe" : "extract") <<  "...";

    QTemporaryFile probeSettings("XXXXXX.tif");
    probeSettings.open();

    m_outData.clear();
    m_errData.clear();

    QStringList args;
    buildArgs( probe ? probeSettings.fileName() : "", args );
    qDebug() << "UfrawProcess::run()" << args;
    setArguments( args );
    start();

    if( probe )
    {
        waitForFinished(-1);
        QString probePath = probeSettings.fileName();
        probePath = probePath.left( probePath.length() - 3 ) + "ufraw";
        QFile probeFile(probePath);
        loadProbeSettings( probeFile );
        probeFile.remove();
    }
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

void UfrawProcess::setWbTemperature(int wbTemperature)
{
    m_wbTemperature = wbTemperature;
}

void UfrawProcess::setWbGreen(double wbGreen)
{
    m_wbGreen = wbGreen;
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

void UfrawProcess::buildArgs(QString probePath, QStringList &args)
{
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

    args.clear();
    args << "--overwrite" << "--zip"<< "--noexif"
         << "--out-depth=16" << "--out-type=ppm"
         << "--lensfun=auto"
         << "--auto-crop"
         << "--rotate=camera"
         << QString("--shrink=%1").arg(m_shrink)
         << QString("--exposure=%1").arg(m_exposure)
         << QString("--interpolation=%1").arg(interpolate)
         << QString("--restore=%1").arg(restore)
         << QString("--clip=%1").arg(clip)
         << "--color-smoothing";

    if( m_wbTemperature && m_wbGreen )
        args << QString("--temperature=%1").arg(m_wbTemperature) << QString("--green=%1").arg(m_wbGreen);
    else
        args << "--wb=camera";

    if( probePath.isEmpty() )
        args << "--create-id=no" << "--output=-";
    else
        args << "--create-id=only" << QString("--output=%1").arg(probePath);

    args << m_raw;
}

void UfrawProcess::loadProbeSettings(QFile &file)
{
    qDebug() << "UfrawProcess::loadProbeSettings()" << file.fileName();
    if( !file.open(QFile::ReadOnly) ) {
        qDebug() << "UfrawProcess::loadProbeSettings() open failed";
        return;
    }
    QString xml = QString::fromUtf8(file.readAll());
    QRegularExpression re("<(?P<tag>\\w+)>\\s*(?P<val>.+?)\\s*</(?P=tag)>",QRegularExpression::MultilineOption);
    if( !re.isValid() )
        qDebug() << "UfrawProcess::loadProbeSettings() invalid re:" << re.errorString();

    QStringList knownInfo;
    knownInfo << "Make" << "Model" << "Lens" << "LensModel"
              << "ISOSpeed" << "Shutter" << "Aperture" << "FocalLength";
    m_info.clear();

    QRegularExpressionMatchIterator it = re.globalMatch(xml);
    while( it.hasNext() )
    {
        QRegularExpressionMatch m = it.next();
        QString tag = m.captured("tag");
        QString val = m.captured("val");
        qDebug() <<  tag << val;
        if( tag == "Temperature" )
            m_wbTemperature = val.toInt();
        else if( tag == "Green" )
            m_wbGreen = val.toDouble();
        else if( knownInfo.indexOf(tag) != -1 )
            m_info[tag] = QString::fromUtf8(val.toUtf8());
    }
    file.close();
    qDebug() << "UfrawProcess::loadProbeSettings()" << m_info;
}
