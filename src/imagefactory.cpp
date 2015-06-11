#include "imagefactory.h"

ImageFactory::ImageFactory(WorkerBase *worker, QObject *parent)
    : QObject(parent)
    , m_image(worker)
    , m_underExposed(worker)
    , m_overExposed(worker)
{
    connect( &m_image,        SIGNAL(urlChanged(QUrl)), this, SIGNAL(imageChanged(QUrl)) );
    connect( &m_underExposed, SIGNAL(urlChanged(QUrl)), this, SIGNAL(underExposedChanged(QUrl)) );
    connect( &m_overExposed,  SIGNAL(urlChanged(QUrl)), this, SIGNAL(overExposedChanged(QUrl)) );
}

ImageFactory::~ImageFactory()
{
    // EMPTY
}
