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
    under.write( "/Users/manuel/tmp/test_under.tif" );
    return WorkerBase::convert( under );
}

