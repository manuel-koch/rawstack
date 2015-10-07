#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "imagecachebase.h"

#include <Magick++.h>

#include <QObject>
#include <QTemporaryDir>
#include <QMap>
#include <QFileInfo>

class ImageCache
    : public QObject
    , public ImageCacheBase
{
    Q_OBJECT

public:

    explicit ImageCache(QObject *parent = NULL);
    virtual ~ImageCache();

    // ImageCacheBase interface
    virtual Magick::Image retrieve(const std::string &key) override;
    virtual void store(const std::string &key, Magick::Image img) override;

private:

    QString hash(const std::string &key);
    void reduce();

private:

    QTemporaryDir   m_tmpDir;
    QList<QString>  m_cached;
    size_t          m_sizeMb;
    size_t          m_maxSizeMb;
};

#endif // IMAGECACHE_H
