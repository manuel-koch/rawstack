#include "workerbase.h"
#include "configbase.h"
#include "imagecachebase.h"

#include <QDebug>
#include <QMutexLocker>

QImage WorkerBase::convert(Magick::Image image)
{
    Magick::Blob blob;
    image.write( &blob );
    return QImage::fromData( QByteArray( static_cast<const char*>(blob.data()), blob.length() ) );
}

WorkerBase::WorkerBase()
    : QObject(NULL)
    , m_common(NULL)
    , m_cache(NULL)
    , m_mutex(QMutex::Recursive)
    , m_progress(0)
    , m_cycle(0)
    , m_config(NULL)
{
    connect( this, SIGNAL(develop(bool,WorkerBase*)), this, SLOT(onDevelop(bool,WorkerBase*)) );
}

WorkerBase::~WorkerBase()
{
    QMutexLocker lock(&m_mutex);
    qDebug() << "WorkerBase::~WorkerBase()" << this;
}

void WorkerBase::setConfig(ConfigBase *config)
{
    if( m_config )
        disconnect( m_config, SIGNAL(hashChanged()), this, SLOT(onCfgHashChanged()) );

    m_config = config;

    if( m_config )
        connect( m_config, SIGNAL(hashChanged()), this, SLOT(onCfgHashChanged()) );
}

void WorkerBase::setCache(ImageCacheBase *cache)
{
    m_cache = cache;
}

void WorkerBase::releaseImages()
{
    QMutexLocker lock(&m_mutex);

    m_img     = Magick::Image();
    m_preview = Magick::Image();
}

void WorkerBase::onCfgHashChanged()
{
    QMutexLocker lock(&m_mutex);

    QByteArray hash = m_config->hash();
    bool dirty = m_doneConfigHash != hash;
    qDebug() << "WorkerBase::onCfgHashChanged()" << this << (dirty?"dirty":"clean");
    setDirty( dirty );
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
    QByteArray preImgHash = predecessor ? predecessor->hash() : QByteArray();
    QByteArray curCfgHash = m_config->hash();
    QByteArray curImgHash = m_config->hash( preImgHash );
    if( m_doneImgHash != curImgHash )
    {
        if( m_cache )
        {
            m_img = m_cache->retrieve( curImgHash.toHex().toStdString() );
            imgCached = m_img.isValid();
        }
        if( !imgCached )
        {
            m_img = predecessor ? predecessor->gmimage() : Magick::Image();
            if( (!predecessor || m_img.isValid()) && m_config->enabled() )
                developImpl( preview, predecessor );
            if( m_cache )
                m_cache->store( curImgHash.toHex().toStdString(), m_img );
        }
        m_doneConfigHash = curCfgHash;
        m_doneImgHash    = curImgHash;
        onCfgHashChanged(); // re-evaluate dirty flag based on current config
    }

    if( m_img.isValid() )
    {
        qDebug() << "WorkerBase::onDevelop()" << this
                 << "result image:" << m_img.format().c_str()
                 << "width:"  << m_img.size().width()
                 << "height:" << m_img.size().height();
        Magick::Image img = m_img;
        img.write( QString("/Users/manuel/tmp/test_%1.tif").arg(m_config->name()).toStdString() );
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
