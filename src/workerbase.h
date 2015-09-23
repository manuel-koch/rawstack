#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <Magick++.h>

#include <QObject>
#include <QImage>

class ConfigBase;

class WorkerBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int    cycle    READ cycle    NOTIFY cycleChanged)

public:

    explicit WorkerBase();
    virtual ~WorkerBase();

    void setConfig(ConfigBase *config) { m_config = config; }

    double progress() const { return m_progress; }
    int    cycle()    const { return m_cycle; }
    const QByteArray &hash() const { return m_imgHash; }

    virtual const Magick::Image gmimage() { return m_img; }
    static QImage convert(Magick::Image image);

    template <typename T> T *config() { return qobject_cast<T*>(m_config); }

signals:

    void progressChanged(double progress);
    void cycleChanged(int cycle);

    void develop( bool preview, WorkerBase *predecessor );
    void started();
    void finished();

protected slots:

    void setProgress(double progress);

private slots:

    void onDevelop( bool preview, WorkerBase *predecessor );

private:

    virtual void prepareImpl();
    virtual void developImpl(bool preview, WorkerBase *predecessor );
    void nextCycle();

protected:

    Magick::Image m_img;

private:

    double      m_progress;
    int         m_cycle;
    ConfigBase *m_config;
    QByteArray  m_imgHash;
};

#endif // WORKERBASE_H
