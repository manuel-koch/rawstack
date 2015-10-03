#include "imagefactory.h"

ImageFactory::ImageFactory(WorkerBase *worker, QObject *parent)
    : QObject(parent)
    , m_image(worker)
    , m_preview(worker)
    , m_underExposed(worker)
    , m_overExposed(worker)
    , m_histFactory(worker)
{
    connect( &m_image,        SIGNAL(urlChanged(QUrl)), this, SIGNAL(imageChanged(QUrl)) );
    connect( &m_preview,      SIGNAL(urlChanged(QUrl)), this, SIGNAL(previewChanged(QUrl)) );
    connect( &m_underExposed, SIGNAL(urlChanged(QUrl)), this, SIGNAL(underExposedChanged(QUrl)) );
    connect( &m_overExposed,  SIGNAL(urlChanged(QUrl)), this, SIGNAL(overExposedChanged(QUrl)) );
}

ImageFactory::~ImageFactory()
{
    // EMPTY
}
