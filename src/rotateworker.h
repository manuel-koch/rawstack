#ifndef UFRAWWORKER_H
#define UFRAWWORKER_H

#include "workerbase.h"

class RotateWorker : public WorkerBase
{
    Q_OBJECT

public:

    explicit RotateWorker();
    virtual ~RotateWorker();

private:

    // WorkerBase interface
    virtual void registerSettingsImpl() override;
    virtual void developImpl(bool preview, WorkerBase *predecessor) override;
};

#endif // ROTATEWORKER_H
