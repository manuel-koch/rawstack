#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <Magick++.h>

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

    virtual const Magick::Image gmimage() { return m_img; }

    template <typename T> T *config() { return qobject_cast<T*>(m_config); }

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

protected:

    Magick::Image m_img;

private:

    double      m_progress;
    ConfigBase *m_config;

};

#endif // WORKERBASE_H
