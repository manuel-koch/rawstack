#ifndef TASKBASE_H
#define TASKBASE_H

#include "configdbentry.h"
#include "workerbase.h"
#include "imagefactory.h"

#include <QObject>
#include <QThread>

class TaskBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString        name       READ name       CONSTANT)
    Q_PROPERTY(ConfigDbEntry* config     READ config     CONSTANT)
    Q_PROPERTY(ImageFactory*  images     READ images     CONSTANT)
    Q_PROPERTY(double         progress   READ progress   NOTIFY progressChanged)
    Q_PROPERTY(bool           dirty      READ dirty      NOTIFY dirtyChanged)
    Q_PROPERTY(bool           enabled    READ enabled    WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool           canDisable READ canDisable CONSTANT)

public:

    explicit TaskBase(QString name = "", QObject *parent = NULL);
    virtual ~TaskBase();

    QString        name() { return m_name; }
    ConfigDbEntry* config() { return m_config; }
    ImageFactory*  images() { return m_images; }
    WorkerBase*    worker() { return m_worker; }

    double progress() const { return m_worker ? m_worker->progress() : 0; }
    bool   dirty() { return m_worker ? m_worker->dirty() : false; }
    bool   enabled();
    virtual bool canDisable() { return true; }

    void setConfig( ConfigDbEntry *config );
    void setWorker( WorkerBase *worker );

    void setEnabled(bool enabled);

signals:

    void progressChanged(double progress);
    void dirtyChanged(bool dirty);
    void enabledChanged(bool enabled);

    void started();
    void finished();

public slots:

    void develop( bool preview, TaskBase *predecessor = NULL );

private slots:

    void onEnabledChanged(QVariant enabled);
    void onStarted();
    void onFinished();

private:

    void initBaseSettings();
    virtual void initTaskSettings();

private:

    QString        m_name;
    ConfigDbEntry *m_config;
    WorkerBase    *m_worker;
    ImageFactory  *m_images;
};

#endif // TASKBASE_H
