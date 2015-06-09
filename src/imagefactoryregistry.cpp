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

