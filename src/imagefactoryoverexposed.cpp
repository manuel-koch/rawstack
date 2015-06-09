#include "imagefactoryoverexposed.h"
#include "workerbase.h"

ImageFactoryOverExposed::ImageFactoryOverExposed(WorkerBase *worker)
    : ImageFactoryBase(worker)
{
    // EMPTY
}

ImageFactoryOverExposed::~ImageFactoryOverExposed()
{
    // EMPTY
}

QImage ImageFactoryOverExposed::image()
{
    if( !m_worker->gmimage().isValid() )
        return QImage();

    Magick::Image over( m_worker->gmimage() );
    over.threshold( 0.99*MaxRGB );
    over.transparent( "black" );
    over.type( Magick::TrueColorMatteType );
    return WorkerBase::convert( over );
}

