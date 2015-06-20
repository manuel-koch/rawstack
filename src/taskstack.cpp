#include "taskstack.h"
#include "taskbase.h"
#include "configbase.h"
#include "commontasks.h"
#include "configfilesaver.h"
#include "configfileloader.h"
#include "taskfactory.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>

TaskStack::TaskStack(QObject *parent)
    : QAbstractListModel(parent)
    , m_commonTasks(NULL)
    , m_progress(0)
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
    delete m_commonTasks;
}

void TaskStack::addTask(TaskBase *task, int idx)
{
    if( m_tasks.indexOf(task) != -1 )
        return;

    if( idx < 0 || idx>m_tasks.size() )
        idx = m_tasks.size(); // append at end

    qDebug() << "TaskStack::addTask()" << task << idx;

    beginInsertRows( QModelIndex(), idx, 1 );

    m_tasks.insert( idx, task );
    connect( task, SIGNAL(started()),               this, SLOT(onTaskStarted()) );
    connect( task, SIGNAL(progressChanged(double)), this, SLOT(onTaskProgress(double)) );
    connect( task, SIGNAL(finished()),              this, SLOT(onTaskFinished()) );

    m_commonTasks->setFinal( m_tasks.back() );
    if( task->config()->name() == "ufraw" )
        m_commonTasks->setUfraw(task);

    endInsertRows();
}

void TaskStack::develop()
{
    qDebug() << "TaskStack::develop()";
    if( !m_tasks.empty() )
        m_tasks[0]->develop();
}

void TaskStack::saveToFile(QString path)
{
    if( m_tasks.empty() )
        return;

    ConfigBase *ufraw = m_commonTasks->ufraw()->config();
    QFileInfo rawInfo( ufraw->property("raw").toString() );
    qDebug() << "TaskStack::saveToFile()" << rawInfo.absoluteFilePath();

    QFileInfo cfgInfo;
    if( path.isEmpty() )
    {
        cfgInfo = QFileInfo( rawInfo.dir(), rawInfo.completeBaseName() + ".rawstack" );
    }
    else
    {
        QFileInfo pathInfo(path);
        cfgInfo = QFileInfo( pathInfo.dir(), pathInfo.completeBaseName() + ".rawstack" );
    }

    setConfig( cfgInfo.absoluteFilePath() );
    qDebug() << "TaskStack::saveToFile()" << m_config;
    ConfigFileSaver fileSaver;
    fileSaver.add("raw",rawInfo.absoluteFilePath());
    std::for_each( m_tasks.begin(), m_tasks.end(), [&] (TaskBase *task) {
        fileSaver.add( task->config() );
    });
    fileSaver.save(m_config);
}

void TaskStack::loadFromFile(QString path)
{
    qDebug() << "TaskStack::loadFromFile()" << path;
    QFileInfo cfgInfo(path);
    cfgInfo = QFileInfo( cfgInfo.dir(), cfgInfo.completeBaseName() + ".rawstack" );

    clearTasks();

    QMap<QString,QString> settings;
    ConfigFileLoader loader;
    connect( &loader, &ConfigFileLoader::setting, [&] (QString key, QString value) {
        qDebug() << "TaskStack::loadFromFile()" << key << value;
        settings[key] = value;
    });
    connect( &loader, &ConfigFileLoader::config, [&] (ConfigBase *cfg) {
        qDebug() << "TaskStack::loadFromFile()" << cfg;
        addTask( TaskFactory::getInstance()->create(cfg) );
    });
    if( loader.load(cfgInfo.absoluteFilePath()) )
    {
        if( settings.contains("raw") )
            m_commonTasks->ufraw()->config()->setProperty("raw",settings["raw"]);
    }
    else
    {
        qDebug() << "TaskStack::loadFromFile() failed";
    }
}

void TaskStack::setProgress(double progress)
{
    if( m_progress == progress )
        return;
    m_progress = progress;
    qDebug() << "TaskStack::setProgress()" << m_progress;
    emit progressChanged(progress);
}

void TaskStack::setConfig(QString config)
{
    if( m_config == config )
        return;
    m_config = config;
    qDebug() << "TaskStack::setConfig()" << m_config;
    emit configChanged(m_config);
}

void TaskStack::clearTasks()
{
    beginResetModel();

    std::for_each( m_tasks.begin(), m_tasks.end(), [] (TaskBase *task) {
        delete task;
    });
    m_tasks.clear();

    endResetModel();
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
    double p = (double)(idx+1) / m_tasks.size();
    setProgress( p );
    int nextIdx = idx+1;
    if( nextIdx >= m_tasks.size() )
        setDeveloping(false);
    else
        m_tasks[nextIdx]->develop();
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

