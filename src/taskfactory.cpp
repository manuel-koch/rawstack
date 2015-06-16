#include "taskfactory.h"
#include "taskbuilderbase.h"
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
    auto it = m_builder.find(builder->name());
    if( it != m_builder.end() )
    {
        qDebug() << "TaskFactory::add()" << builder->name() << "already added";
        return false;
    }
    qDebug() << "TaskFactory::add()" << builder->name();
    m_builder[builder->name()] = builder;
    return true;
}

TaskBase *TaskFactory::create(ConfigBase *config)
{
    auto it = m_builder.find(config->name());
    if( it == m_builder.end() )
    {
        qDebug() << "TaskFactory::create()" << config << "unknown";
        return NULL;
    }

    TaskBuilderBase *builder = (*it);
    TaskBase *task = builder->create(this,m_thread);
    qDebug() << "TaskFactory::create()" << task;
    task->setConfig( config );
    return task;
}

