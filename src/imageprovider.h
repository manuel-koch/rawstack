#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    ~ImageProvider();

    // QQuickImageProvider interface
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    static const QString name;
};

#endif // IMAGEPROVIDER_H
