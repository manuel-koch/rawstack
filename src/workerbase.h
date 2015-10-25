#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <Magick++.h>

#include <QObject>
#include <QMutex>
#include <QImage>

class ConfigBase;
class CommonConfig;
class ImageCache;

class WorkerBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int    cycle    READ cycle    NOTIFY cycleChanged)
    Q_PROPERTY(bool   dirty    READ dirty    NOTIFY dirtyChanged)

public:

    explicit WorkerBase();
    virtual ~WorkerBase();

    void setCommonConfig(CommonConfig *common) { m_common = common; }
    void setConfig(ConfigBase *config);
    void setCache(ImageCache *cache);

    double progress() const { return m_progress; }
    int    cycle()    const { return m_cycle; }
    bool   dirty()    const { return m_dirty; }

    const QByteArray &hash() const { return m_doneImgHash; }

    virtual const Magick::Image gmimage() { return m_img; }
    virtual const Magick::Image gmpreview() { return m_preview; }
    virtual void releaseImages();
    static QImage convert(Magick::Image image);

    CommonConfig *common() { return m_common; }
    template <typename T> T *config() { return qobject_cast<T*>(m_config); }

signals:

    void progressChanged(double progress);
    void cycleChanged(int cycle);
    void dirtyChanged(bool dirty);

    void develop( bool preview, WorkerBase *predecessor );
    void started();
    void finished();

protected slots:

    void setProgress(double progress);

private slots:

    void setDirty( bool dirty );
    void onDevelop( bool preview, WorkerBase *predecessor );
    void onCfgHashChanged();

private:

    virtual void prepareImpl();
    virtual void developImpl(bool preview, WorkerBase *predecessor );
    void nextCycle();

protected:

    CommonConfig   *m_common;
    ImageCache     *m_cache;
    Magick::Image   m_img;
    Magick::Image   m_preview;

private:

    QMutex         m_mutex;
    double         m_progress;
    int            m_cycle;
    bool           m_dirty;
    ConfigBase    *m_config;
    QByteArray     m_doneConfigHash;
    QByteArray     m_doneImgHash;
};

#endif // WORKERBASE_H
