#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "imagecachegroup.h"
#include "thumbnailloader.h"

#include <Magick++.h>

#include <QObject>
#include <QThread>
#include <QFileInfo>

class ImageCacheEntry;

class ImageCache : public QObject
{
    Q_OBJECT
    Q_ENUMS(Size)

public:

    explicit ImageCache(QObject *parent = NULL);
    virtual ~ImageCache();

    void store(QString key, ImageCacheGroup::Lifetime lifetime, Magick::Image img);
    void store(QByteArray key, ImageCacheGroup::Lifetime lifetime, Magick::Image img);

    bool isCached(QString key);
    Magick::Image retrieve(QString key, bool thumbnail);
    Magick::Image retrieve(QByteArray key, bool thumbnail);

signals:

    /// An image has been updated in the cache by given action
    void cached( ImageCacheGroup::Action action, QString key );

    /// An image was not found in cache
    void miss( QString key );

    /// Trigger loading of thumbnail for given RAW image
    void loadThumbnail( QString path );

private:

    ImageCacheGroup &group( ImageCacheGroup::Lifetime lifetime );

private:

    QThread         m_thread;
    ThumbnailLoader m_thumbLoader;
    ImageCacheGroup m_tmp;
    ImageCacheGroup m_persist;
};

#endif // IMAGECACHE_H
