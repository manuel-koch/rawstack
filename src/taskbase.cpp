#include "taskbase.h"
#include "workerbase.h"

#include <QDebug>

TaskBase::TaskBase(QString name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_config(NULL)
    , m_worker(NULL)
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
}

void TaskBase::setWorker(WorkerBase *worker)
{
    m_worker = worker;
    connect( m_worker, SIGNAL(started()),               this, SIGNAL(started()) );
    connect( m_worker, SIGNAL(progressChanged(double)), this, SIGNAL(progressChanged(double)) );
    connect( m_worker, SIGNAL(finished()),              this, SIGNAL(finished()) );
}

void TaskBase::develop(TaskBase *predecessor)
{
    qDebug() << "TaskBase::develop()" << m_name << predecessor;
   if( m_worker )
   {
       WorkerBase *predecWorder = predecessor ? predecessor->worker() : NULL;
       m_worker->develop( predecWorder );
   }
}

void TaskBase::setName(QString name)
{
    if (m_name == name)
        return;
    m_name = name;
    emit nameChanged(m_name);
}

void TaskBase::onStarted()
{
    qDebug() << "TaskBase::onStarted()" << m_name;
}

void TaskBase::onFinished()
{
    qDebug() << "TaskBase::onFinished()" << m_name;
}

