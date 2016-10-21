/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#include "ufrawprocess.h"

#include <QDebug>
#include <QTemporaryFile>
#include <QRegularExpression>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>

UfrawProcess::UfrawProcess(QObject *parent)
    : QProcess(parent)
    , m_output()
    , m_failed(false)
    , m_shrink(1)
    , m_exposure(0)
    , m_colorSmoothing(true)
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
    connect( this, SIGNAL(readyReadStandardOutput()),            this, SLOT(onConsole()) );
}

UfrawProcess::~UfrawProcess()
{
    if( state() != QProcess::Running )
        waitForFinished();
}

void UfrawProcess::run(Output output)
{
    if( state() == QProcess::Running )
        return;

    m_failed = false;

    qDebug() << "UfrawProcess::run()" << output;

    if( output == OutputThumbnail )
        m_output.setFileTemplate(QDir::tempPath()+QDir::separator()+"XXXXXX.jpg");
    else
        m_output.setFileTemplate(QDir::tempPath()+QDir::separator()+"XXXXXX.tif");
    if( !m_output.open() )
        qDebug() << "UfrawProcess::run() open temporary file failed:" << m_output.errorString();
    m_console.clear();

    QStringList args;
    buildArgs( output, args );
    qDebug() << "UfrawProcess::run()" << program() << args;
    setArguments( args );
    setProcessChannelMode( QProcess::MergedChannels );
    start();

    if( output != OutputImage )
        waitForFinished(-1);
    if( output == OutputProbe )
        loadProbeSettings();
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

void UfrawProcess::setColorSmoothing(bool smooth)
{
    m_colorSmoothing = smooth;
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
    m_failed = true;
}

void UfrawProcess::onConsole()
{
    m_console.append( readAllStandardOutput() );
    emit progress();
}

void UfrawProcess::buildArgs(Output output, QStringList &args)
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

    args << "--overwrite" << "--zip"<< "--noexif";

    if( output == OutputThumbnail )
    {
        args << "--embedded-image";
    }
    else
    {
        args << "--out-depth=16" << "--out-type=ppm"
             << "--lensfun=auto"
             << "--auto-crop"
             << "--rotate=camera"
             << QString("--shrink=%1").arg(m_shrink)
             << QString("--exposure=%1").arg(m_exposure)
             << QString("--interpolation=%1").arg(interpolate)
             << QString("--restore=%1").arg(restore)
             << QString("--clip=%1").arg(clip);

        if( m_colorSmoothing )
             args << "--color-smoothing";

        if( m_wbTemperature && m_wbGreen )
            args << QString("--temperature=%1").arg(m_wbTemperature) << QString("--green=%1").arg(m_wbGreen);
        else
            args << "--wb=camera";
    }

    if( output == OutputProbe )
        args << "--create-id=only";
    else
        args << "--create-id=no";

    args << QString("--output=%1").arg(m_output.fileName());
    args << m_raw;
}

void UfrawProcess::loadProbeSettings()
{
    QString probePath = m_output.fileName();
    probePath = probePath.left( probePath.length() - 3 ) + "ufraw";
    QFile probeFile(probePath);
    qDebug() << "UfrawProcess::loadProbeSettings()" << probeFile.fileName();

    m_wbTemperature = 0;
    m_wbGreen       = 0;
    m_info.clear();

    if( !probeFile.open( QFile::ReadOnly ) )
    {
        qDebug() << "UfrawProcess::loadProbeSettings() failed to read file";
        probeFile.remove();
        return;
    }

    QDomDocument doc;
    QString err;
    int errLine, errCol;
    bool res = doc.setContent(&probeFile,true,&err,&errLine,&errCol);
    probeFile.close();
    probeFile.remove();
    if( !res )
    {
        qDebug() << "UfrawProcess::loadProbeSettings() error at line" << errLine << "col" << errCol << ":" << err;
        return;
    }

    QDomElement rootElm        = doc.documentElement();
    QDomElement temperatureElm = rootElm.firstChildElement("Temperature").toElement();
    QDomElement greenElm       = rootElm.firstChildElement("Green").toElement();

    if( temperatureElm.isElement() )
    {
        m_wbTemperature = temperatureElm.text().toInt();
        qDebug() << "UfrawProcess::loadProbeSettings() Temperatur" << m_wbTemperature;
    }
    if( greenElm.isElement() )
    {
        m_wbGreen = greenElm.text().toFloat();
        qDebug() << "UfrawProcess::loadProbeSettings() Green" << m_wbGreen;
    }

    QStringList knownInfos;
    knownInfos << "Make" << "Model" << "Lens" << "LensModel"
               << "ISOSpeed" << "Shutter" << "Aperture" << "FocalLength";
    foreach( QString info, knownInfos )
    {
        QDomElement infoElm = rootElm.firstChildElement(info).toElement();
        if( infoElm.isElement() )
        {
            m_info[info] = infoElm.text();
            qDebug() << "UfrawProcess::loadProbeSettings()" << info << m_info[info];
        }
    }
}
