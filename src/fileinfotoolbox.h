#ifndef FILEINFOTOOLBOX_H
#define FILEINFOTOOLBOX_H

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

        QStringList validTypes;
        validTypes << "image/x-canon-cr2";
        return validTypes.indexOf(mimeType.name()) != -1;
    }
};

#endif // FILEINFOTOOLBOX_H

