#ifndef TASKBASE_H
#define TASKBASE_H

#include "workerbase.h"

#include <QObject>

class QThread;
class ConfigBase;

class TaskBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConfigBase* config   READ config   CONSTANT)
    Q_PROPERTY(double      progress READ progress NOTIFY progressChanged)

public:
    explicit TaskBase(QObject *parent = NULL);
    virtual ~TaskBase();

    ConfigBase* config() const { return m_config; }
    WorkerBase* worker() const { return m_worker; }
    double      progress() const { return m_worker ? m_worker->progress() : 0; }

    void setConfig( ConfigBase *config );
    void setWorker( WorkerBase *worker );

signals:

    void progressChanged(double progress);

    void started();
    void finished();

public slots:

    void develop( TaskBase *predecessor = NULL );

private slots:

    void onStarted();
    void onFinished();

private:

    ConfigBase *m_config;
    WorkerBase *m_worker;
};

#endif // TASKBASE_H
