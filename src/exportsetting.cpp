#include "exportsetting.h"
#include "configdbentry.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

ExportSetting::ExportSetting(ConfigDbEntry *config, QObject *parent)
    : QObject(parent)
    , m_config(config)
{
    setImgPath( m_config->config() );
}

ExportSetting::~ExportSetting()
{
    // EMPTY
}

void ExportSetting::setImgPath(QString imgPath)
{
    imgPath = adjustedImgPath( imgPath );
    if( m_imgPath == imgPath )
        return;

    m_imgPath = imgPath;
    qDebug() << "ExportSetting::setImgPath()" << m_imgPath;
    emit imgPathChanged(imgPath);
}

void ExportSetting::setImgType(ExportSetting::ImageType imgType)
{
    if( m_imgType == imgType )
        return;

    m_imgType = imgType;
    qDebug() << "ExportSetting::setImgType()" << m_imgType;
    emit imgTypeChanged(imgType);

    setImgPath( adjustedImgPath(m_imgPath) );
}

void ExportSetting::setImgQuality(int imgQuality)
{
    imgQuality = std::min( 100, std::max( 1, imgQuality) );
    if( m_imgQuality == imgQuality )
        return;

    m_imgQuality = imgQuality;
    qDebug() << "ExportSetting::setImgQuality()" << m_imgQuality;
    emit imgQualityChanged(imgQuality);
}

QString ExportSetting::adjustedImgPath(QString path)
{
    QFileInfo info( path );
    QString ext;
    switch( m_imgType )
    {
        case ExportSetting::TIF:
        {
            ext = ".tif";
            break;
        }
        case ExportSetting::PNG:
        {
            ext = ".png";
            break;
        }
        case ExportSetting::JPG:
        default:
        {
            ext = ".jpg";
            break;
        }
    }
    return QFileInfo( info.dir(), info.completeBaseName() + ext ).absoluteFilePath();
}

