#ifndef IMAGEFACTORYWORKER_H
#define IMAGEFACTORYWORKER_H

#include "imagefactorybase.h"

#include <QObject>
#include <QUrl>
#include <QImage>

class WorkerBase;

class ImageFactoryWorker : public ImageFactoryBase
{
    Q_OBJECT

public:

    explicit ImageFactoryWorker(WorkerBase *worker);
    virtual ~ImageFactoryWorker();

    virtual QImage image() override;

private slots:

    void onCycleChanged(int cycle);

protected:

    WorkerBase *m_worker;
};

#endif // IMAGEFACTORYWORKER_H
