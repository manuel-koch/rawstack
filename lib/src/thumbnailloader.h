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
#ifndef THUMBNAILLOADER_H
#define THUMBNAILLOADER_H

#include <QObject>

class ImageCache;

class ThumbnailLoader : public QObject
{
    Q_OBJECT

public:

    explicit ThumbnailLoader(ImageCache &cache, QObject *parent = 0);

signals:

    void load(QString path);

public slots:

    void loadImpl(QString path);

private:

    ImageCache &m_cache;
};

#endif // THUMBNAILLOADER_H
