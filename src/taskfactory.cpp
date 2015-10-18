#include "taskfactory.h"
#include "taskbuilderbase.h"
#include "configbuilderbase.h"
#include "configbase.h"
#include "taskbase.h"
#include "imagecache.h"

#include <QDebug>
#include <QThread>

TaskFactory *TaskFactory::ms_instance = NULL;

TaskFactory::TaskFactory(QObject *parent)
    : QObject(parent)
{
    m_imgCache = new ImageCache(this);
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
    while( !m_configBuilder.empty() )
    {
        delete m_configBuilder.first();
        m_configBuilder.erase( m_configBuilder.begin() );
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

bool TaskFactory::add(ConfigBuilderBase *builder)
{
    auto it = m_configBuilder.find(builder->name());
    if( it != m_configBuilder.end() )
    {
        qDebug() << "TaskFactory::add() config" << builder->name() << "already added";
        return false;
    }
    qDebug() << "TaskFactory::add() config" << builder->name();
    m_configBuilder[builder->name()] = builder;
    return true;
}

ConfigBase *TaskFactory::create(QString config)
{
    auto it = m_configBuilder.find(config);
    if( it == m_configBuilder.end() )
    {
        qDebug() << "TaskFactory::create() config" << config << "unknown";
        return NULL;
    }

    ConfigBuilderBase *builder = (*it);
    ConfigBase *cfg = builder->create();
    qDebug() << "TaskFactory::create() " << cfg;
    return cfg;
}

TaskBase *TaskFactory::create(ConfigBase *config, QThread *workerThread)
{
    auto it = m_taskBuilder.find(config->name());
    if( it == m_taskBuilder.end() )
    {
        qDebug() << "TaskFactory::create() task" << config << "unknown";
        return NULL;
    }

    if( !workerThread )
        workerThread = m_workerThread;

    TaskBuilderBase *builder = (*it);
    TaskBase *task = builder->create( this, workerThread );
    qDebug() << "TaskFactory::create()" << task;
    task->setConfig( config );
    task->worker()->setCache( m_imgCache );
    return task;
}
