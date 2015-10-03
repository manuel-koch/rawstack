#ifndef IMAGEFACTORYPREVIEW_H
#define IMAGEFACTORYPREVIEW_H

#include "imagefactoryworker.h"

class ImageFactoryPreview : public ImageFactoryWorker
{
    Q_OBJECT

public:

    explicit ImageFactoryPreview(WorkerBase *worker);
    virtual ~ImageFactoryPreview();

    // ImageFactoryBase interface
    virtual QImage image() override;
};

#endif // IMAGEFACTORYPREVIEW_H
