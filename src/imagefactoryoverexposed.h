#ifndef IMAGEFACTORYOVEREXPOSED_H
#define IMAGEFACTORYOVEREXPOSED_H

#include "imagefactoryworker.h"

class ImageFactoryOverExposed : public ImageFactoryWorker
{
    Q_OBJECT

public:

    explicit ImageFactoryOverExposed(WorkerBase *worker);
    virtual ~ImageFactoryOverExposed();

    // ImageFactoryBase interface
    virtual QImage image() override;
};

#endif // IMAGEFACTORYOVEREXPOSED_H
