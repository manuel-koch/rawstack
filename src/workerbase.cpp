#include "workerbase.h"
#include "configdbentry.h"
#include "configsetting.h"
#include "imagecache.h"
#include "histogrammaker.h"

#include <QDebug>
#include <QMutexLocker>
#include <QCryptographicHash>

QImage WorkerBase::convert(Magick::Image image)
{
    Magick::Blob blob;
    image.write( &blob );
    return QImage::fromData( QByteArray( static_cast<const char*>(blob.data()), blob.length() ) );
}

WorkerBase::WorkerBase(QString name)
    : QObject(NULL)
    , m_cache(NULL)
    , m_mutex(QMutex::Recursive)
    , m_name(name)
    , m_progress(0)
    , m_cycle(0)
    , m_config(NULL)
    , m_histogram(NULL)
{
    m_histogram = new HistogramMaker(this);
    connect( this, &WorkerBase::develop,          this, &WorkerBase::onDevelop );
    connect( this, &WorkerBase::requestHistogram, this, &WorkerBase::onRequestHistogram );
}

WorkerBase::~WorkerBase()
{
    QMutexLocker lock(&m_mutex);
    qDebug() << "WorkerBase::~WorkerBase()" << this;
}

void WorkerBase::setConfig(ConfigDbEntry *config)
{
    qDebug() << "WorkerBase::setConfig()" << this << config;
    m_config = config;
    hashSetting( m_name+".enabled" );
    registerSettingsImpl();
}

void WorkerBase::registerSettingsImpl()
{
    // EMPTY
}

void WorkerBase::hashSetting(QString name)
{
    QMutexLocker lock(&m_mutex);

    ConfigSetting *setting = m_config->settings()->getSetting(name);
    if( m_hashSettings.contains(setting) )
        return;

    qDebug() << "WorkerBase::hashSetting()" << this << setting->fullname() << setting->value();
    m_hashSettings.append(setting);
    connect( setting, SIGNAL(valueChanged(QVariant)), this, SLOT(onSettingChanged()) );
}

QByteArray WorkerBase::hashSettings(QByteArray init)
{
    QMutexLocker lock(&m_mutex);

    qDebug() << "WorkerBase::hashSettings()" << this;
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( init );
    h.addData( m_name.toLocal8Bit() );
    foreach( ConfigSetting *setting, m_hashSettings )
    {
        QString value = setting->value().toString();
        qDebug() << "WorkerBase::hashSettings()" << this << setting->fullname() << value;
        h.addData( value.toLocal8Bit() );
    }
    return h.result();
}

void WorkerBase::onSettingChanged()
{
    QByteArray hash = hashSettings();
    bool dirty = m_doneSettingsHash != hash;
    qDebug() << "WorkerBase::onSettingChanged()" << this << (dirty?"dirty":"clean");
    setDirty( dirty );
}

void WorkerBase::onRequestHistogram()
{
    qDebug() << "WorkerBase::onRequestHistogram()" << this;
    m_histogram->analyze( m_img );
}

void WorkerBase::setCache(ImageCache *cache)
{
    m_cache = cache;
}

void WorkerBase::releaseImages()
{
    QMutexLocker lock(&m_mutex);

    m_img     = Magick::Image();
    m_preview = Magick::Image();
}

void WorkerBase::setProgress(double progress)
{
    QMutexLocker lock(&m_mutex);

    progress = std::min( progress, 1.0 );
    progress = std::max( progress, 0.0 );
    if (m_progress == progress)
        return;
    m_progress = progress;
    qDebug() << "WorkerBase::setProgress()" << this << m_progress;
    emit progressChanged(m_progress);
}

void WorkerBase::setDirty(bool dirty)
{
    QMutexLocker lock(&m_mutex);

    if( m_dirty == dirty )
        return;
    m_dirty = dirty;
    qDebug() << "WorkerBase::setDirty()" << this << m_dirty;
    emit dirtyChanged(m_dirty);
}

void WorkerBase::onDevelop(bool preview, WorkerBase *predecessor)
{
    QMutexLocker lock(&m_mutex);

    qDebug() << "WorkerBase::onDevelop()" << this << (preview ? "preview" : "HQ") << predecessor;

    emit started();
    setProgress(0);

    prepareImpl();

    bool imgCached = false;
    bool enabled = config()->settings()->getSetting(m_name+".enabled")->value().toBool();

    QByteArray preImgHash = predecessor ? predecessor->hash() : QByteArray();
    QByteArray curSettingsHash = hashSettings();
    QByteArray curImgHash = hashSettings( preImgHash );
    if( m_doneImgHash != curImgHash )
    {
        if( m_cache )
        {
            m_img = m_cache->retrieve( curImgHash, false );
            imgCached = m_img.isValid();
        }
        if( !imgCached )
        {
            m_img = predecessor ? predecessor->gmimage() : Magick::Image();
            if( (!predecessor || m_img.isValid()) && enabled )
                developImpl( preview, predecessor );
            if( m_cache )
                m_cache->store( curImgHash, ImageCacheGroup::Temporary, m_img );
        }
        else
        {
            qDebug() << "WorkerBase::onDevelop()" << this << "using cached result image";
        }
        m_doneSettingsHash = curSettingsHash;
        m_doneImgHash      = curImgHash;
        onSettingChanged(); // re-evaluate dirty flag based on current settings
    }

    if( m_img.isValid() )
    {
        qDebug() << "WorkerBase::onDevelop()" << this
                 << "result image:" << m_img.format().c_str()
                 << "width:"  << m_img.size().width()
                 << "height:" << m_img.size().height();
        Magick::Image img = m_img;
        img.write( QString("/Users/manuel/tmp/test_%1.tif").arg(m_name).toStdString() );
    }
    else
    {
        qWarning() << "WorkerBase::onDevelop()" << this << "invalid image created";
        m_img = Magick::Image();
    }

    setProgress(1);
    emit finished();
    nextCycle();
}

void WorkerBase::prepareImpl()
{
    qDebug() << "WorkerBase::prepareImpl()" << this;
}

void WorkerBase::developImpl(bool preview, WorkerBase *predecessor)
{
    Q_UNUSED(preview);
    qDebug() << "WorkerBase::developImpl()" << this << predecessor;
}

void WorkerBase::nextCycle()
{
    QMutexLocker lock(&m_mutex);

    m_cycle++;
    qDebug() << "WorkerBase::nextCycle()" << this << m_cycle;
    emit cycleChanged(m_cycle);
}
