#include "taskstack.h"
#include "taskbase.h"
#include "configbase.h"

#include <QDebug>

TaskStack::TaskStack(QObject *parent)
    : QAbstractListModel(parent)
    , m_progress(0)
{
    m_rolemap = QAbstractListModel::roleNames();
    m_rolemap[NameRole]     = "name";
    m_rolemap[TaskRole]     = "task";
    m_rolemap[ConfigRole]   = "config";
    m_rolemap[ProgressRole] = "progress";
}

TaskStack::~TaskStack()
{
    std::for_each( m_tasks.begin(), m_tasks.end(), [] (TaskBase *task) {
        delete task;
    });
    m_tasks.clear();
}

void TaskStack::addTask(TaskBase *task, int idx)
{
    if( m_tasks.indexOf(task) != -1 )
        return;

    if( idx < 0 )
        idx = m_tasks.size(); // append at end

    qDebug() << "TaskStack::addTask()" << task << idx;

    beginInsertRows( QModelIndex(), idx, 1 );

    m_tasks.insert( idx, task );
    connect( task, SIGNAL(started()),               this, SLOT(onTaskStarted()) );
    connect( task, SIGNAL(progressChanged(double)), this, SLOT(onTaskProgress(double)) );
    connect( task, SIGNAL(finished()),              this, SLOT(onTaskFinished()) );

    endInsertRows();
}

void TaskStack::develop()
{
    qDebug() << "TaskStack::develop()";
    if( !m_tasks.empty() )
        m_tasks[0]->develop();
}

void TaskStack::setProgress(double progress)
{
    if( m_progress == progress )
        return;
    m_progress = progress;
    qDebug() << "TaskStack::setProgress()" << m_progress;
    emit progressChanged(progress);
}

int TaskStack::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() )
        return 0;
    return m_tasks.size();
}

QVariant TaskStack::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_tasks.size() )
        return QVariant();

    switch( role )
    {
        case Qt::DisplayRole:
        case NameRole:
        {
            return m_tasks[index.row()]->config()->name();
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
    qDebug() << "TaskStack::onTaskStarted()";
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    qDebug() << "TaskStack::onTaskStarted()" << task << idx;
    double p = (double)idx / m_tasks.size();
    setProgress( p );
}

void TaskStack::onTaskProgress(double progress)
{
    qDebug() << "TaskStack::onTaskProgress()";
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    qDebug() << "TaskStack::onTaskProgress()" << task << idx;
    double p = (double)idx / m_tasks.size() + progress / m_tasks.size();
    setProgress( p );
}

void TaskStack::onTaskFinished()
{
    qDebug() << "TaskStack::onTaskFinished()";
    TaskBase *task = qobject_cast<TaskBase*>( sender() );
    int idx = m_tasks.indexOf( task );
    qDebug() << "TaskStack::onTaskFinished()" << task << idx;
    double p = (double)(idx+1) / m_tasks.size();
    setProgress( p );
}

