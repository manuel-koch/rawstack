#ifndef TASKBASE_H
#define TASKBASE_H

#include "configbase.h"
#include "workerbase.h"
#include "imagefactory.h"

#include <QObject>
#include <QThread>

class TaskBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConfigBase*   config   READ config   CONSTANT)
    Q_PROPERTY(ImageFactory* images   READ images   CONSTANT)
    Q_PROPERTY(double        progress READ progress NOTIFY progressChanged)

public:
    explicit TaskBase(QObject *parent = NULL);
    virtual ~TaskBase();

    ConfigBase*   config() { return m_config; }
    ImageFactory* images() { return m_images; }
    WorkerBase*   worker() { return m_worker; }
    double        progress() const { return m_worker ? m_worker->progress() : 0; }

    void setConfig( ConfigBase *config );
    void setWorker( WorkerBase *worker );

signals:

    void progressChanged(double progress);

    void started();
    void finished();

public slots:

    void develop( bool preview, TaskBase *predecessor = NULL );

private slots:

    void onStarted();
    void onFinished();

private:

    ConfigBase   *m_config;
    WorkerBase   *m_worker;
    ImageFactory *m_images;
};

#endif // TASKBASE_H
