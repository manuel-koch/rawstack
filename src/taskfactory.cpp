#include "taskfactory.h"
#include "taskbuilderbase.h"
#include "taskbase.h"
#include "imagecache.h"

#include <QDebug>
#include <QThread>

TaskFactory *TaskFactory::ms_instance = NULL;

TaskFactory::TaskFactory(QObject *parent)
    : QObject(parent)
{
    m_workerThread = new QThread(this);
    m_workerThread->setObjectName("TaskFactory-WorkerThread");
    m_workerThread->start();
}

TaskFactory::~TaskFactory()
{
    m_workerThread->quit();
    m_workerThread->wait();
    while( !m_taskBuilder.empty() )
    {
        delete m_taskBuilder.first();
        m_taskBuilder.erase( m_taskBuilder.begin() );
    }
}

bool TaskFactory::add(TaskBuilderBase *builder)
{
    auto it = m_taskBuilder.find(builder->name());
    if( it != m_taskBuilder.end() )
    {
        qDebug() << "TaskFactory::add() task" << builder->name() << "already added";
        return false;
    }
    qDebug() << "TaskFactory::add() task" << builder->name();
    m_taskBuilder[builder->name()] = builder;
    return true;
}

TaskBase *TaskFactory::create(QString name, QThread *workerThread)
{
    auto it = m_taskBuilder.find(name);
    if( it == m_taskBuilder.end() )
    {
        qDebug() << "TaskFactory::create() task" << name << "unknown";
        return NULL;
    }

    if( !workerThread )
        workerThread = m_workerThread;

    TaskBuilderBase *builder = (*it);
    TaskBase *task = builder->create( this, workerThread );
    qDebug() << "TaskFactory::create()" << task;
    return task;
}
