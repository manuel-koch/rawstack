#include "imageprovider.h"
#include "imagefactorybase.h"
#include "imagefactoryregistry.h"

#include <QDebug>
#include <QUrl>

const QString ImageProvider::name("rawstack");

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    // EMPTY
}

ImageProvider::~ImageProvider()
{
    // EMPTY
}

QImage ImageProvider::requestImage(const QString &path, QSize *size, const QSize &requestedSize)
{
    qDebug() << "ImageProvider::requestImage(" << path << size << requestedSize << ")";

    QImage image;

    QUrl url(path);
    ImageFactoryBase *factory = ImageFactoryRegistry::getInstance()->getFactory(url.path());
    if( !factory )
        return image;

    image = factory->image();
    if( image.isNull() )
        return image;

    if( size )
        *size = image.size();

    if( requestedSize.isValid() && image.size() != requestedSize )
        image.scaled( requestedSize, Qt::KeepAspectRatio );

    return image;
}

