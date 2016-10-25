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
