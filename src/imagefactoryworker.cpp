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
