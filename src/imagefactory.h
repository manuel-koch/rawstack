#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "imagefactorybase.h"
#include "imagefactoryunderexposed.h"
#include "imagefactoryoverexposed.h"

#include <QObject>

class ImageFactory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ImageFactoryBase* image        READ image        CONSTANT)
    Q_PROPERTY(ImageFactoryBase* underExposed READ underExposed CONSTANT)
    Q_PROPERTY(ImageFactoryBase* overExposed  READ overExposed  CONSTANT)

public:
    explicit ImageFactory(WorkerBase *worker, QObject *parent = NULL);
    ~ImageFactory();

    ImageFactoryBase *image()        { return &m_image; }
    ImageFactoryBase *underExposed() { return &m_underExposed; }
    ImageFactoryBase *overExposed()  { return &m_overExposed; }

private:

    ImageFactoryBase          m_image;
    ImageFactoryUnderExposed  m_underExposed;
    ImageFactoryOverExposed   m_overExposed;
};

#endif // IMAGEFACTORY_H
