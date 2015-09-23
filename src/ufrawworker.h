#ifndef UFRAWWORKER_H
#define UFRAWWORKER_H

#include "workerbase.h"

#include <QObject>

class UfrawWorker : public WorkerBase
{
    Q_OBJECT

public:

    explicit UfrawWorker();
    virtual ~UfrawWorker();

private:

    // WorkerBase interface
    virtual void prepareImpl();
    virtual void developImpl(bool preview, WorkerBase *predecessor);

};

#endif // UFRAWWORKER_H
