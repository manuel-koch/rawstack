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
#include "imagefactorythumbnail.h"
#include "configdb.h"
#include "configdbentry.h"
#include "imagecache.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QFileInfo>

ImageFactoryThumbnail::ImageFactoryThumbnail(ConfigDbEntry *config)
    : m_config(config)
    , m_cycle(0)
{
    connect( m_config, SIGNAL(rawChanged(QString)), this, SLOT(onRawChanged(QString)) );
    if( m_config->db() )
    {
        connect( &m_config->db()->cache(), SIGNAL(miss(QString)), this, SLOT(onCacheMiss(QString)) );
        connect( &m_config->db()->cache(), SIGNAL(cached(ImageCacheGroup::Action,QString)), this, SLOT(onCacheChanged(ImageCacheGroup::Action,QString)) );
    }
    setUrl( m_cycle );
}

void ImageFactoryThumbnail::nextUrl()
{
    m_cycle++;
    setUrl( m_cycle );
}

QImage ImageFactoryThumbnail::image()
{
    // try loading cached thumbnail of exported image
    Magick::Image thumbnail = m_config->db()->cache().retrieve(m_config->config(),true);
    if( !thumbnail.isValid() )
    {
        // fallback to loading cached thumbnail embedded in RAW image
        thumbnail = m_config->db()->cache().retrieve(m_config->raw(),true);
        if( !thumbnail.isValid() )
        {
            qDebug() << "ImageFactoryThumbnail::image() invalid" << m_config->raw();
            return QImage();
        }
    }

    Magick::Blob blob;
    thumbnail.write( &blob );
    qDebug() << "ImageFactoryThumbnail::image()" << thumbnail.size().width() << "x" << thumbnail.size().height();
    return QImage::fromData( QByteArray( static_cast<const char*>(blob.data()), blob.length() ) );
}

void ImageFactoryThumbnail::onRawChanged(QString raw)
{
    Q_UNUSED(raw);
    nextUrl();
}

void ImageFactoryThumbnail::onConfigChanged(QString config)
{
    Q_UNUSED(config);
    nextUrl();
}

void ImageFactoryThumbnail::onCacheChanged(ImageCacheGroup::Action action, QString key)
{
    Q_UNUSED(action);
    if( key == m_config->raw() || key == m_config->config() )
        nextUrl();
}

void ImageFactoryThumbnail::onCacheMiss(QString key)
{
    if( key == m_config->raw() )
        m_config->db()->cache().loadThumbnail(  m_config->raw() );
}
