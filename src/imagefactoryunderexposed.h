#ifndef IMAGEFACTORYUNDEREXPOSED_H
#define IMAGEFACTORYUNDEREXPOSED_H

#include "imagefactoryworker.h"

class ImageFactoryUnderExposed : public ImageFactoryWorker
{
    Q_OBJECT

public:

    explicit ImageFactoryUnderExposed(WorkerBase *worker);
    virtual ~ImageFactoryUnderExposed();

    // ImageFactoryBase interface
    virtual QImage image() override;
};

#endif // IMAGEFACTORYUNDEREXPOSED_H
