/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
