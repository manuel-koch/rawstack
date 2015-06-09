#ifndef IMAGEFACTORYOVEREXPOSED_H
#define IMAGEFACTORYOVEREXPOSED_H

#include "imagefactorybase.h"

class ImageFactoryOverExposed : public ImageFactoryBase
{
    Q_OBJECT

public:

    explicit ImageFactoryOverExposed(WorkerBase *worker);
    virtual ~ImageFactoryOverExposed();

public:

    // ImageFactoryBase interface
    virtual QImage image();
};

#endif // IMAGEFACTORYOVEREXPOSED_H
