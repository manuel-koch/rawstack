#ifndef TASKBASE_H
#define TASKBASE_H

#include "workerbase.h"

#include <QObject>

class QThread;
class ConfigBase;

class TaskBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString     name     READ name     NOTIFY nameChanged)
    Q_PROPERTY(ConfigBase* config   READ config   CONSTANT)
    Q_PROPERTY(double      progress READ progress NOTIFY progressChanged)

public:
    explicit TaskBase(QString name, QObject *parent);
    virtual ~TaskBase();

    QString name() const { return m_name; }
    ConfigBase* config() const { return m_config; }
    WorkerBase* worker() const { return m_worker; }
    double progress() const { return m_worker ? m_worker->progress() : 0; }

    void setConfig( ConfigBase *config );
    void setWorker( WorkerBase *worker );

signals:

    void nameChanged(QString name);
    void progressChanged(double progress);

    void started();
    void finished();

public slots:

    void develop( TaskBase *predecessor = NULL );
    void setName(QString name);

private slots:

    void onStarted();
    void onFinished();

private:

    QString     m_name;
    ConfigBase *m_config;
    WorkerBase *m_worker;
};

#endif // TASKBASE_H
