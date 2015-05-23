#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <QObject>

class ConfigBase;

class WorkerBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)

public:

    explicit WorkerBase(ConfigBase *config);
    virtual ~WorkerBase();

    double progress() const { return m_progress; }

signals:

    void progressChanged(double progress);

    void develop( WorkerBase *predecessor );
    void started();
    void finished();

protected slots:

    void setProgress(double progress);

private slots:

    void onDevelop( WorkerBase *predecessor );

private:

    virtual void developImpl( WorkerBase *predecessor );

private:

    double      m_progress;
    ConfigBase *m_config;
};

#endif // WORKERBASE_H
