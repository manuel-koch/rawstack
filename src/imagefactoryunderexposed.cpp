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
#include "imagefactoryunderexposed.h"
#include "workerbase.h"

ImageFactoryUnderExposed::ImageFactoryUnderExposed(WorkerBase *worker)
    : ImageFactoryWorker(worker)
{
    // EMPTY
}

ImageFactoryUnderExposed::~ImageFactoryUnderExposed()
{
    // EMPTY
}

QImage ImageFactoryUnderExposed::image()
{
    if( !m_worker->gmimage().isValid() )
        return QImage();

    Magick::Image under( m_worker->gmimage() );
    under.threshold( 0.01*MaxRGB );
    under.negate();
    under.transparent( "black" );
    under.type( Magick::TrueColorMatteType );
    return WorkerBase::convert( under );
}

