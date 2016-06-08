#include "exportimgtype.h"

#include <QFileInfo>
#include <QDir>

ExportImgType::ExportImgType(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

QString ExportImgType::adjusted(ExportImgType::ImageType imgType, QString path)
{
    QFileInfo info( path );
    QString ext;
    switch( imgType )
    {
        case ExportImgType::TIF:
        {
            ext = ".tif";
            break;
        }
        case ExportImgType::PNG:
        {
            ext = ".png";
            break;
        }
        case ExportImgType::JPG:
        default:
        {
            ext = ".jpg";
            break;
        }
    }
    return QFileInfo( info.dir(), info.completeBaseName() + ext ).absoluteFilePath();
}
