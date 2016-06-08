#ifndef EXPORTIMGTYPE_H
#define EXPORTIMGTYPE_H

#include <QObject>

class ExportImgType : public QObject
{
    Q_OBJECT
    Q_ENUMS(ImageType)

public:

    explicit ExportImgType(QObject *parent = 0);

    enum ImageType
    {
        JPG,
        TIF,
        PNG
    };

    /// Returns path with proper image type extension
    static QString adjusted(ImageType imgType, QString path);
};

#endif // EXPORTIMGTYPE_H
