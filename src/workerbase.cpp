#include "workerbase.h"

#include <QDebug>

WorkerBase::WorkerBase(ConfigBase *config)
    : QObject(NULL)
    , m_progress(0)
    , m_config(config)
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

    developImpl( predecessor );

    setProgress(1);
    emit finished();
}

void WorkerBase::developImpl(WorkerBase *predecessor)
{
    qDebug() << "WorkerBase::developImpl()" << this << predecessor;
}
