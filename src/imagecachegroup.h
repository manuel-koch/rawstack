#ifndef IMAGECACHEGROUP_H
#define IMAGECACHEGROUP_H

#include "imagecacheentry.h"

#include <Magick++.h>

#include <QObject>
#include <QMutex>
#include <QDir>

class ImageCacheGroup : public QObject
{
    Q_OBJECT
    Q_ENUMS(Lifetime Action)

public:

    enum Lifetime
    {
        Temporary,
        Persistent
    };

    enum Action
    {
        Updated,
        Removed
    };

    explicit ImageCacheGroup(QString name, Lifetime lifetime, QObject *parent = 0);
    ~ImageCacheGroup();

    /// Add / update given image for selected cache key
    void store(QString key, Magick::Image img);

    /// Returns true when selected key is cached
    bool isCached(QString key);

    /// Get cached image ( if any ) for given key
    Magick::Image retrieve(QString key, bool thumbnail);

    /// Save current content of cache for later loading
    void save();

    /// Reload previously cached image info
    void load();

signals:

    /// An image has been updated in the cache by given action
    void cached(ImageCacheGroup::Action action, QString key );

    /// Update image in cache using given key and lifetime
    void triggerStore(QString key, QByteArray blob);

private slots:

    void storeImpl(QString key, QByteArray blob);

    void onMemSizeChanged(long delta);
    void onFileSizeChanged(long delta);

private:

    void addEntry( QString key, QString path );

    /// Apply size constraints on cache to free memory and disk space
    void cleanup();

private:

    QMutex                          m_mutex;
    QString                         m_name;
    Lifetime                        m_lifetime;
    QDir                            m_dir;
    QHash<QString,ImageCacheEntry*> m_cached;
    QList<QString>                  m_used;
    unsigned long                   m_memsize;
    unsigned long                   m_maxMemSize;
    unsigned long                   m_filesize;
    unsigned long                   m_maxFileSize;
};

#endif // IMAGECACHEGROUP_H
