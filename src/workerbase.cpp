#include "workerbase.h"
#include "configbase.h"

#include <QDebug>

QImage WorkerBase::convert(Magick::Image image)
{
    Magick::Blob blob;
    image.write( &blob, "tif" );
    return QImage::fromData( QByteArray( static_cast<const char*>(blob.data()), blob.length() ) );
}

WorkerBase::WorkerBase()
    : QObject(NULL)
    , m_progress(0)
    , m_cycle(0)
    , m_config(NULL)
{
    connect( this, SIGNAL(develop(bool,WorkerBase*)), this, SLOT(onDevelop(bool,WorkerBase*)) );
}

WorkerBase::~WorkerBase()
{
    // EMPTY
}

void WorkerBase::setProgress(double progress)
{
    progress = std::min( progress, 1.0 );
    progress = std::max( progress, 0.0 );
    if (m_progress == progress)
        return;
    m_progress = progress;
    qDebug() << "WorkerBase::setProgress()" << this << m_progress;
    emit progressChanged(m_progress);
}

void WorkerBase::onDevelop(bool preview, WorkerBase *predecessor)
{
    qDebug() << "WorkerBase::onDevelop()" << this << (preview ? "preview" : "HQ") << predecessor;

    emit started();
    setProgress(0);

    prepareImpl();

    QByteArray preHash = predecessor ? predecessor->hash() : QByteArray();
    QByteArray curHash = m_config->hash( preHash );
    if( m_imgHash != curHash )
    {
        m_img = predecessor ? predecessor->gmimage() : Magick::Image();
        if( (!predecessor || m_img.isValid()) && m_config->enabled() )
            developImpl( preview, predecessor );
        m_imgHash = curHash;
    }
    else
    {
        qDebug() << "WorkerBase::onDevelop()" << this << "same hash, using current image";
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

    m_config->resetDirty();
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
    m_cycle++;
    qDebug() << "WorkerBase::nextCycle()" << this << m_cycle;
    emit cycleChanged(m_cycle);
}
