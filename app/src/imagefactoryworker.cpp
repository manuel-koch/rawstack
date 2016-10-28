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
#include "imagefactoryworker.h"
#include "imagefactoryregistry.h"
#include "imageprovider.h"
#include "workerbase.h"

#include <QDebug>
#include <QUrlQuery>
#include <QUuid>

ImageFactoryWorker::ImageFactoryWorker(WorkerBase *worker)
    : ImageFactoryBase()
    , m_worker(worker)
{
    connect( m_worker, SIGNAL(cycleChanged(int)), this, SLOT(onCycleChanged(int)) );
    onCycleChanged( 0 );
}

ImageFactoryWorker::~ImageFactoryWorker()
{
    // EMPTY
}

QImage ImageFactoryWorker::image()
{
    if( m_worker->gmimage().isValid() )
    {
        qDebug() << "ImageFactoryBase::image()" << m_worker->gmimage().size().width() << "x" << m_worker->gmimage().size().height();
        return WorkerBase::convert( m_worker->gmimage() );
    }
    else
        return QImage();
}

void ImageFactoryWorker::onCycleChanged(int cycle)
{
    setUrl( cycle );
}
