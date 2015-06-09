#include "imagefactory.h"

ImageFactory::ImageFactory(WorkerBase *worker, QObject *parent)
    : QObject(parent)
    , m_image(worker)
    , m_underExposed(worker)
    , m_overExposed(worker)
{
    // EMPTY
}

ImageFactory::~ImageFactory()
{
    // EMPTY
}
