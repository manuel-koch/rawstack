#include "taskfactory.h"
#include "taskbuilderbase.h"
#include "configbuilderbase.h"
#include "configbase.h"
#include "taskbase.h"

#include <QDebug>
#include <QThread>

TaskFactory::TaskFactory(QObject *parent) : QObject(parent)
{
    m_thread = new QThread(this);
    m_thread->start();
}

TaskFactory::~TaskFactory()
{
    m_thread->quit();
    m_thread->wait();
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

TaskBase *TaskFactory::create(ConfigBase *config)
{
    auto it = m_taskBuilder.find(config->name());
    if( it == m_taskBuilder.end() )
    {
        qDebug() << "TaskFactory::create() task" << config << "unknown";
        return NULL;
    }

    TaskBuilderBase *builder = (*it);
    TaskBase *task = builder->create(this,m_thread);
    qDebug() << "TaskFactory::create()" << task;
    task->setConfig( config );
    return task;
}

