#include "taskbase.h"
#include "workerbase.h"

#include <QDebug>

TaskBase::TaskBase(QObject *parent)
    : QObject(parent)
    , m_config(NULL)
    , m_worker(NULL)
    , m_images(NULL)
{
    connect( this, SIGNAL(started()),  this, SLOT(onStarted()) );
    connect( this, SIGNAL(finished()), this, SLOT(onFinished()) );
}

TaskBase::~TaskBase()
{
    // EMPTY
}

void TaskBase::setConfig(ConfigBase *config)
{
    m_config = config;
    if( m_config )
        m_config->setParent(this);
    m_worker->setConfig(m_config);
}

void TaskBase::setWorker(WorkerBase *worker)
{
    m_worker = worker;
    connect( m_worker, SIGNAL(started()),               this, SIGNAL(started()) );
    connect( m_worker, SIGNAL(finished()),              this, SIGNAL(finished()) );
    connect( m_worker, SIGNAL(progressChanged(double)), this, SIGNAL(progressChanged(double)) );
    connect( m_worker, SIGNAL(dirtyChanged(bool)),      this, SIGNAL(dirtyChanged(bool)) );
    m_images = new ImageFactory(worker,this);
}

void TaskBase::develop(bool preview, TaskBase *predecessor)
{
    qDebug() << "TaskBase::develop()" << this << predecessor;
   if( m_worker )
   {
       WorkerBase *predecWorder = predecessor ? predecessor->worker() : NULL;
       m_worker->develop( preview, predecWorder );
   }
}

void TaskBase::onStarted()
{
    qDebug() << "TaskBase::onStarted()" << this;
}

void TaskBase::onFinished()
{
    qDebug() << "TaskBase::onFinished()" << this;
}
