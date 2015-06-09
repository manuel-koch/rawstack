#ifndef IMAGEFACTORYUNDEREXPOSED_H
#define IMAGEFACTORYUNDEREXPOSED_H

#include "imagefactorybase.h"

class ImageFactoryUnderExposed : public ImageFactoryBase
{
    Q_OBJECT

public:

    explicit ImageFactoryUnderExposed(WorkerBase *worker);
    virtual ~ImageFactoryUnderExposed();

public:

    // ImageFactoryBase interface
    virtual QImage image();
};

#endif // IMAGEFACTORYUNDEREXPOSED_H
