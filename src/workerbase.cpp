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
    connect( this, SIGNAL(develop(WorkerBase*)), this, SLOT(onDevelop(WorkerBase*)) );
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

void WorkerBase::onDevelop(WorkerBase *predecessor)
{
    qDebug() << "WorkerBase::onDevelop()" << this << predecessor;

    emit started();
    setProgress(0);

    QByteArray preHash = predecessor ? predecessor->hash() : QByteArray();
    QByteArray imgHash = m_config->hash( preHash );
    qDebug() << "WorkerBase::onDevelop()" << this << "curr" << imgHash.toHex();
    qDebug() << "WorkerBase::onDevelop()" << this << "last" << m_imgHash.toHex();
    if( m_imgHash != imgHash )
    {
        m_img = Magick::Image();
        developImpl( predecessor );
        m_imgHash = imgHash;
    }

    m_config->resetDirty();
    setProgress(1);
    emit finished();
    nextCycle();
}

void WorkerBase::developImpl(WorkerBase *predecessor)
{
    qDebug() << "WorkerBase::developImpl()" << this << predecessor;
}

void WorkerBase::nextCycle()
{
    m_cycle++;
    qDebug() << "WorkerBase::nextCycle()" << this << m_cycle;
    emit cycleChanged(m_cycle);
}
