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
#ifndef IMAGEFACTORYTHUMBNAIL_H
#define IMAGEFACTORYTHUMBNAIL_H

#include "imagefactorybase.h"
#include "imagecachegroup.h"

class ConfigDbEntry;

class ImageFactoryThumbnail : public ImageFactoryBase
{
    Q_OBJECT

public:

    explicit ImageFactoryThumbnail(ConfigDbEntry *config);

public:

    // ImageFactoryBase interface
    virtual QImage image();

private slots:

    void onRawChanged(QString raw);
    void onConfigChanged(QString config);
    void onCacheChanged(ImageCacheGroup::Action action, QString key);
    void onCacheMiss(QString key);

    void nextUrl();

private:

    ConfigDbEntry *m_config;
    int            m_cycle;
};

#endif // IMAGEFACTORYTHUMBNAIL_H
