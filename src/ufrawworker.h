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

    void run(UfrawProcess &ufraw, bool lowQuality, int idx, int nof);

    // WorkerBase interface
    virtual void registerSettingsImpl() override;
    virtual void prepareImpl() override;
    virtual void developImpl(bool preview, WorkerBase *predecessor) override;
};

#endif // UFRAWWORKER_H
