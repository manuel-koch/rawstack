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

