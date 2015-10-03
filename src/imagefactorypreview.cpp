#include "imagefactorypreview.h"
#include "workerbase.h"

ImageFactoryPreview::ImageFactoryPreview(WorkerBase *worker)
    : ImageFactoryWorker(worker)
{
    // EMPTY
}

ImageFactoryPreview::~ImageFactoryPreview()
{
    // EMPTY
}

QImage ImageFactoryPreview::image()
{
    if( !m_worker->gmpreview().isValid() )
        return QImage();

    return WorkerBase::convert( m_worker->gmpreview() );
}

