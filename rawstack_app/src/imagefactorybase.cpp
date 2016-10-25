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
#include "imagefactorybase.h"
#include "imagefactoryregistry.h"
#include "imageprovider.h"
#include "workerbase.h"

#include <QDebug>
#include <QUrlQuery>
#include <QUuid>

QString create_uuid()
{
    QString uuid( QUuid::createUuid().toString() );
    uuid.remove(0,1);
    uuid.remove(uuid.size()-1,1);
    return uuid;
}

ImageFactoryBase::ImageFactoryBase()
    : QObject(NULL)
    , m_id(create_uuid())
{
    ImageFactoryRegistry::getInstance()->registerFactory(this);
}

ImageFactoryBase::~ImageFactoryBase()
{
    ImageFactoryRegistry::getInstance()->unregisterFactory(this);
}

QImage ImageFactoryBase::image()
{
    return QImage();
}

void ImageFactoryBase::setUrl(int hash)
{
    QUrl url;
    url.setScheme("image");
    url.setHost( ImageProvider::name );
    url.setPath(QString("/%1").arg(m_id));
    QUrlQuery query;
    query.addQueryItem("hash",QString("%1").arg(hash));
    url.setQuery(query);
    if( url != m_url )
    {
        m_url = url;
        qDebug() << "ImageFactoryBase::setUrl()" << this << m_url;
        emit urlChanged(m_url);
    }
}

void ImageFactoryBase::setUrl(const QByteArray &hash)
{
    QUrl url;
    url.setScheme("image");
    url.setHost( ImageProvider::name );
    url.setPath(QString("/%1").arg(m_id));
    QUrlQuery query;
    query.addQueryItem("hash",QString("%1").arg(QString::fromUtf8(hash.toHex())));
    url.setQuery(query);
    if( url != m_url )
    {
        m_url = url;
        qDebug() << "ImageFactoryBase::setUrl()" << this << hash.toHex();
        emit urlChanged(m_url);
    }
}

