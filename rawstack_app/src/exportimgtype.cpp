#include "exportimgtype.h"

#include <QFileInfo>
#include <QDir>

ExportImgType::ExportImgType(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

QString ExportImgType::ext(ExportImgType::ImageType imgType)
{
    switch( imgType )
    {
        case ExportImgType::TIF: return "tif";
        case ExportImgType::PNG: return "png";
        case ExportImgType::JPG: return "jpg";
        return "";
    }
}

QString ExportImgType::adjusted(ExportImgType::ImageType imgType, QString path)
{
    QFileInfo info( path );
    return QFileInfo( info.dir(), info.completeBaseName() + "." + ext(imgType) ).absoluteFilePath();
}
