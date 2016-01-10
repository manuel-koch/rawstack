#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <Magick++.h>

#include <QObject>
#include <QMutex>
#include <QImage>

class ConfigDbEntry;
class ConfigSetting;
class ImageCache;
class HistogramMaker;

class WorkerBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int    cycle    READ cycle    NOTIFY cycleChanged)
    Q_PROPERTY(bool   dirty    READ dirty    NOTIFY dirtyChanged)

public:

    explicit WorkerBase(QString name);
    virtual ~WorkerBase();

    void setConfig(ConfigDbEntry *config);
    void setCache(ImageCache *cache);

    QString name() const { return m_name; }
    ConfigDbEntry* config() const { return m_config; }
    double progress() const { return m_progress; }
    int    cycle()    const { return m_cycle; }
    bool   dirty()    const { return m_dirty; }

    const QByteArray &hash() const { return m_doneImgHash; }

    virtual const Magick::Image gmimage() { return m_img; }
    virtual void releaseImages();
    static QImage convert(Magick::Image image);

    HistogramMaker *histogram() const { return m_histogram; }

signals:

    void progressChanged(double progress);
    void cycleChanged(int cycle);
    void dirtyChanged(bool dirty);

    void develop( bool preview, WorkerBase *predecessor );
    void started();
    void finished();

    void requestHistogram();

protected slots:

    void setProgress(double progress);
    void hashSetting( QString name );

private slots:

    void setDirty( bool dirty );
    void onDevelop( bool preview, WorkerBase *predecessor );
    void onSettingChanged();
    void onRequestHistogram();
    
private:

    virtual void prepareImpl();
    virtual void developImpl(bool preview, WorkerBase *predecessor );
    void nextCycle();

    virtual void registerSettingsImpl();
    QByteArray hashSettings( QByteArray init = QByteArray() );

protected:

    ImageCache     *m_cache;
    Magick::Image   m_img;
    Magick::Image   m_preview;

private:

    QMutex         m_mutex;
    QString        m_name;
    double         m_progress;
    int            m_cycle;
    bool           m_dirty;

    ConfigDbEntry  *m_config;
    HistogramMaker *m_histogram;

    QList<ConfigSetting*> m_hashSettings;
    QByteArray            m_doneSettingsHash;
    QByteArray            m_doneImgHash;
};

#endif // WORKERBASE_H
