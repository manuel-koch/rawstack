#ifndef LEVELWORKER_H
#define LEVELWORKER_H

#include "workerbase.h"

class LevelWorker : public WorkerBase
{
    Q_OBJECT

public:

    explicit LevelWorker();
    virtual ~LevelWorker();

private:

    // WorkerBase interface
    virtual void registerSettingsImpl() override;
    virtual void developImpl(bool preview, WorkerBase *predecessor) override;
};

#endif // LEVELWORKER_H
