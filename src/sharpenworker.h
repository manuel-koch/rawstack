#ifndef SHARPENWORKER_H
#define SHARPENWORKER_H

#include "workerbase.h"

class SharpenWorker : public WorkerBase
{
    Q_OBJECT

public:

    explicit SharpenWorker();
    virtual ~SharpenWorker();

private:

    // WorkerBase interface
    virtual void registerSettingsImpl() override;
    virtual void developImpl(bool preview, WorkerBase *predecessor) override;
};

#endif // SHARPENWORKER_H
