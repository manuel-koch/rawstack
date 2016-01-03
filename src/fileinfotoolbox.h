#ifndef FILEINFOTOOLBOX_H
#define FILEINFOTOOLBOX_H

#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>

class FileInfoToolbox
{
public:

    static bool isRaw( const QFileInfo &path )
    {
        if( path.isDir() )
            return false;

        QMimeDatabase mime;
        QMimeType mimeType = mime.mimeTypeForFile(path);

        qDebug() << "FileInfoToolbox::isRaw(" << path.completeBaseName() << ")" << mimeType.name();

        QStringList validTypes;
        validTypes << "image/x-canon-cr2"
                   << "image/x-nikon-nef"
                   << "image/x-sony-arw"
                   << "image/x-panasonic-raw"
                   << "image/x-panasonic-raw2"
                   << "image/x-pentax-pef";
        return validTypes.indexOf(mimeType.name()) != -1;
    }
};

#endif // FILEINFOTOOLBOX_H

