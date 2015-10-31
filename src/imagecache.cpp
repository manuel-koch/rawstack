#include "imagecache.h"
#include "thumbnailloader.h"

#include <QDebug>

ImageCache::ImageCache( QObject *parent )
    : QObject(parent)
    , m_thumbLoader(*this)
    , m_tmp("tmp",ImageCacheGroup::Temporary)
    , m_persist("persist",ImageCacheGroup::Persistent)
{
    m_thread.setObjectName("ImageCache-ThumbLoader");
    m_thread.start();
    m_thumbLoader.moveToThread( &m_thread );
    connect( this, SIGNAL(loadThumbnail(QString)), &m_thumbLoader, SIGNAL(load(QString)) );
    connect( &m_tmp, SIGNAL(cached(ImageCacheGroup::Action,QString)), this, SIGNAL(cached(ImageCacheGroup::Action,QString)) );
    connect( &m_persist, SIGNAL(cached(ImageCacheGroup::Action,QString)), this, SIGNAL(cached(ImageCacheGroup::Action,QString)) );
    m_persist.load();
}

ImageCache::~ImageCache()
{
    m_thread.quit();
    m_thread.wait();

    m_persist.save();
}

void ImageCache::store(QString key, ImageCacheGroup::Lifetime lifetime, Magick::Image img)
{
    group(lifetime).store( key, img );
}

Magick::Image ImageCache::retrieve(QString key, bool thumbnail)
{
    Magick::Image img = m_persist.retrieve(key,thumbnail);
    if( img.isValid() )
        return img;

    img = m_tmp.retrieve(key,thumbnail);
    if( img.isValid() )
        return img;

    qDebug() << "ImageCache::retrieve() miss" << key;
    emit miss(key);
    return img;
}

void ImageCache::store(QByteArray key, ImageCacheGroup::Lifetime lifetime, Magick::Image img)
{
    store( QString::fromUtf8(key.toHex()), lifetime, img );
}

bool ImageCache::isCached(QString key)
{
    return m_persist.isCached(key) || m_tmp.isCached(key);
}

Magick::Image ImageCache::retrieve(QByteArray key, bool thumbnail)
{
    return retrieve( QString::fromUtf8(key.toHex()), thumbnail );
}

ImageCacheGroup &ImageCache::group(ImageCacheGroup::Lifetime lifetime)
{
    switch(lifetime)
    {
        case ImageCacheGroup::Temporary:  return m_tmp;
        case ImageCacheGroup::Persistent: return m_persist;
    }
    Q_ASSERT(false); // unknown lifetime
}
