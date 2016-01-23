/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
