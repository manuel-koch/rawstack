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
#include "taskstack.h"
#include "taskbase.h"
#include "commontasks.h"
#include "taskfactory.h"
#include "configdb.h"
#include "configdbentry.h"
#include "fileinfotoolbox.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMimeDatabase>

TaskStack::TaskStack(bool preview, QObject *parent)
    : QAbstractListModel(parent)
    , m_workerThread(NULL)
    , m_commonTasks(NULL)
    , m_developing(false)
    , m_dirty(false)
    , m_progress(0)
    , m_preview(preview)
{
    m_rolemap = QAbstractListModel::roleNames();
    m_rolemap[NameRole]     = "name";
    m_rolemap[TaskRole]     = "task";
    m_rolemap[ConfigRole]   = "config";
    m_rolemap[Developing]   = "developing";
    m_rolemap[ProgressRole] = "progress";

    m_commonTasks = new CommonTasks(this);
}

TaskStack::~TaskStack()
{
    clearTasks();
    delete m_commonTasks;
}

void TaskStack::addTask(TaskBase *task, int idx)
{
    if( m_tasks.indexOf(task) != -1 )
        return;

    if( idx < 0 || idx>m_tasks.size() )
        idx = m_tasks.size(); // append at end

    qDebug() << "TaskStack::addTask()" << task << "at" << idx;

    beginInsertRows( QModelIndex(), idx, idx );

    task->setConfig( m_config );
    task->worker()->setCache( &m_config->db()->cache() ); // FIXME: redundant when worker already knows config
    m_tasks.insert( idx, task );
    connect( task, SIGNAL(started()),               this, SLOT(onTaskStarted()) );
    connect( task, SIGNAL(progressChanged(double)), this, SLOT(onTaskProgress(double)) );
    connect( task, SIGNAL(finished()),              this, SLOT(onTaskFinished()) );
    connect( task, SIGNAL(dirtyChanged(bool)),      this, SLOT(onTaskDirty(bool)) );

    m_commonTasks->setFinal( m_tasks.back() );
    if( task->name() == "ufraw" )
        m_commonTasks->setUfraw(task);

    setDirty( anyTaskDirty() );

    endInsertRows();

    qDebug() << "TaskStack::addTask() nof tasks" << m_tasks.size();
}

void TaskStack::removeTask(int idx)
{
    if( idx >= m_tasks.size() )
        return;

    qDebug() << "TaskStack::removeTask()" << idx;

    beginRemoveRows( QModelIndex(), idx, idx );
    TaskBase *task = m_tasks.takeAt(idx);

    disconnect( task, SIGNAL(started()),               this, SLOT(onTaskStarted()) );
    disconnect( task, SIGNAL(progressChanged(double)), this, SLOT(onTaskProgress(double)) );
    disconnect( task, SIGNAL(finished()),              this, SLOT(onTaskFinished()) );
    disconnect( task, SIGNAL(dirtyChanged(bool)),      this, SLOT(onTaskDirty(bool)) );

    if( m_tasks.empty() )
        m_commonTasks->setFinal( NULL );
    else
        m_commonTasks->setFinal( m_tasks.back() );
    if( task->name() == "ufraw" )
        m_commonTasks->setUfraw(NULL);

    setDirty( anyTaskDirty() );

    delete task;
    endRemoveRows();

    qDebug() << "TaskStack::removeTask() nof tasks" << m_tasks.size();
}

Magick::Image TaskStack::gmimage()
{
    if( !m_dirty && m_tasks.size() )
        return m_tasks.back()->worker()->gmimage();
    else
        return Magick::Image();
}

void TaskStack::releaseImages()
{
    foreach( TaskBase *task, m_tasks )
    {
        task->worker()->releaseImages();
    }
}

void TaskStack::develop()
{
    qDebug() << "TaskStack::develop()" << (m_preview ? "LQ" : "HQ");
    if( !m_tasks.empty() && !m_developing )
        m_tasks[0]->develop( m_preview );
}

void TaskStack::setConfig(ConfigDbEntry *config)
{
    if( m_config == config || (m_config && m_config->equals(config)) )
        return;

    qDebug() << "TaskStack::loadFromFile()" << (config ? config->config() : "unload");

    clearTasks();

    if( m_config )
    {
        disconnect( m_config, SIGNAL(destroyed(QObject*)), this, SLOT(onConfigDestroyed()) );
    }

    m_config = config;
    createDefaultTasks();

    if( m_config )
    {
        connect( m_config, SIGNAL(destroyed(QObject*)), this, SLOT(onConfigDestroyed()) );
    }

    emit configChanged(m_config);
}

void TaskStack::setProgress(double progress)
{
    if( m_progress == progress )
        return;
    m_progress = progress;
    qDebug() << "TaskStack::setProgress()" << m_progress;
    emit progressChanged(progress);
}

void TaskStack::setDirty(bool dirty)
{
    if( m_dirty == dirty )
        return;
    m_dirty = dirty;
    qDebug() << "TaskStack::setDirty()" << m_dirty;
    emit dirtyChanged(m_dirty);
}

void TaskStack::clearTasks()
{
    qDebug() << "TaskStack::clearTasks()";

    beginResetModel();
    while( m_tasks.size() )
        removeTask( m_tasks.size()-1 );
    endResetModel();

    setDirty( false );
    setDeveloping( false );
}

void TaskStack::createDefaultTasks()
{
    qDebug() << "TaskStack::createDefaultTasks()" << m_config->raw();

    if( !m_config->isValidRaw() )
    {
        qDebug() << "TaskStack::createDefaultTasks() unsupported raw";
        return;
    }

    QStringList tasks;
    tasks << "ufraw" << "level" << "rotate" << "sharpen";
    foreach( QString task, tasks )
    {
        qDebug() << "TaskStack::createDefaultTasks()" << task;
        addTask( TaskFactory::getInstance()->create( task, m_workerThread ) );
    }
}

bool TaskStack::anyTaskDirty()
{
    bool dirty = false;
    std::for_each( m_tasks.begin(), m_tasks.end(), [&] (TaskBase *task)
    {
        if( task->dirty() )
            dirty = true;
    });
    return dirty;
}

int TaskStack::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() )
        return 0;
    qDebug() << "TaskStack::rowCount()" << m_tasks.size();
    return m_tasks.size();
}

QVariant TaskStack::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_tasks.size() )
        return QVariant();

    qDebug() << "TaskStack::data()" << index.row() << m_rolemap[role];
    switch( role )
    {
        case Qt::DisplayRole:
        case NameRole:
        {
            return m_tasks[index.row()]->name();
        }
        case ProgressRole:
        {
            return progress();
        }
        case TaskRole:
        {
            return QVariant::fromValue<QObject*>( m_tasks[index.row()] );
        }
        case ConfigRole:
        {
            return QVariant::fromValue<QObject*>( m_tasks[index.row()]->config() );
        }
    }
    return QVariant();
}

void TaskStack::onTaskStarted()
{
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    if( idx == -1 )
        return;
    setDeveloping(true);
    qDebug() << "TaskStack::onTaskStarted()" << task << idx;
    double p = (double)idx / m_tasks.size();
    setProgress( p );
}

void TaskStack::onTaskProgress(double progress)
{
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    if( idx == -1 )
        return;
    qDebug() << "TaskStack::onTaskProgress()" << task << idx;
    double p = (double)idx / m_tasks.size() + progress / m_tasks.size();
    setProgress( p );
}

void TaskStack::onTaskFinished()
{
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    if( idx == -1 )
        return;
    qDebug() << "TaskStack::onTaskFinished()" << task << idx;

    for( int i=0; i<=idx; i++ )
    {
        if( m_tasks[i]->dirty() )
        {
            qDebug() << "TaskStack::onTaskFinished()" << i << "is dirty, redo...";
            setDeveloping( false );
            develop();
            return;
        }
    }

    int releaseIdx = idx-1;
    if( !m_preview && releaseIdx >= 0 )
        m_tasks[releaseIdx]->worker()->releaseImages();

    double p = (double)(idx+1) / m_tasks.size();
    setProgress( p );
    int nextIdx = idx+1;
    bool isLastTask = nextIdx >= m_tasks.size();
    if( isLastTask )
    {
        m_config->save();
        m_config->db()->cache().store( m_config->config(),
                                       ImageCacheGroup::Persistent,
                                       m_tasks.back()->worker()->gmimage() );
        setDeveloping( false );
    }
    else
        m_tasks[nextIdx]->develop( m_preview, m_tasks[idx] );
}

void TaskStack::onTaskDirty(bool dirty)
{
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    if( idx == -1 )
        return;

    qDebug() << "TaskStack::onTaskDirty()" << task << idx << (dirty?"dirty":"clean") << "while" << (m_developing?"developing":"idle");

    setDirty( anyTaskDirty() );
    if( m_dirty && !m_developing )
        develop();
}

void TaskStack::onConfigDestroyed()
{
    setConfig( NULL );
}

void TaskStack::setDeveloping(bool developing)
{
    if( m_developing != developing )
    {
        m_developing = developing;
        qDebug() << "TaskStack::setDeveloping()" << m_developing;
        emit developingChanged(m_developing);
    }
}
