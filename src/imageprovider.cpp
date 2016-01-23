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

