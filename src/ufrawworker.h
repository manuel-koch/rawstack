#ifndef UFRAWWORKER_H
#define UFRAWWORKER_H

#include "workerbase.h"

#include <QObject>

class UfrawProcess;

class UfrawWorker : public WorkerBase
{
    Q_OBJECT

public:

    explicit UfrawWorker();
    virtual ~UfrawWorker();

private:

    void run( UfrawProcess &ufraw, bool preview, int idx, int nof );

    // WorkerBase interface
    virtual void prepareImpl();
    virtual void developImpl(bool preview, WorkerBase *predecessor);

};

#endif // UFRAWWORKER_H
