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
#include "imagefactoryregistry.h"
#include "imagefactorybase.h"

#include <QDebug>

ImageFactoryRegistry * ImageFactoryRegistry::ms_instance = NULL;

ImageFactoryRegistry::ImageFactoryRegistry(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

ImageFactoryRegistry::~ImageFactoryRegistry()
{
    // EMPTY
}

void ImageFactoryRegistry::registerFactory(ImageFactoryBase *factory)
{
    if( m_factories.contains(factory->id()) )
        return;
    m_factories[factory->id()] = factory;
}

void ImageFactoryRegistry::unregisterFactory(ImageFactoryBase *factory)
{
    if( !m_factories.contains(factory->id()) )
        return;
    m_factories.remove(factory->id());
}

ImageFactoryBase *ImageFactoryRegistry::getFactory(QString id)
{
    qDebug() << "ImageFactoryRegistry::getFactory(" << id << ")";
    if( m_factories.contains(id) )
        return m_factories[id];
    return NULL;
}

