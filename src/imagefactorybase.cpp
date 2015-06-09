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

ImageFactoryBase::ImageFactoryBase(WorkerBase *worker)
    : QObject(NULL)
    , m_worker(worker)
    , m_id(create_uuid())
{
    ImageFactoryRegistry::getInstance()->registerFactory(this);
    connect( m_worker, SIGNAL(cycleChanged(int)), this, SLOT(onCycleChanged(int)) );
}

ImageFactoryBase::~ImageFactoryBase()
{
    ImageFactoryRegistry::getInstance()->unregisterFactory(this);
}

QImage ImageFactoryBase::image()
{
    if( m_worker->gmimage().isValid() )
    {
        qDebug() << "ImageFactoryBase::image()" << m_worker->gmimage().size().width() << "x" << m_worker->gmimage().size().height();
        return WorkerBase::convert( m_worker->gmimage() );
    }
    else
        return QImage();
}

void ImageFactoryBase::onCycleChanged(int cycle)
{
    QUrl url;
    url.setScheme("image");
    url.setHost( ImageProvider::name );
    url.setPath(QString("/%1").arg(m_id));
    QUrlQuery query;
    query.addQueryItem("cycle",QString("%1").arg(cycle));
    url.setQuery(query);
    if( url != m_url )
    {
        m_url = url;
        qDebug() << "ImageFactoryBase::onCycleChanged()" << m_url;
        emit urlChanged(m_url);
    }
}

