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
    virtual void developImpl(WorkerBase *predecessor);

};

#endif // ROTATEWORKER_H
