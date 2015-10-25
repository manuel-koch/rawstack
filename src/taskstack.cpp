#include "taskstack.h"
#include "taskbase.h"
#include "configbase.h"
#include "commontasks.h"
#include "commonconfig.h"
#include "configfilesaver.h"
#include "configfileloader.h"
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
    , m_commonConfig(NULL)
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
    delete m_commonConfig;
}

void TaskStack::addTask(TaskBase *task, int idx)
{
    if( m_tasks.indexOf(task) != -1 )
        return;

    if( idx < 0 || idx>m_tasks.size() )
        idx = m_tasks.size(); // append at end

    qDebug() << "TaskStack::addTask()" << task << "at" << idx;

    beginInsertRows( QModelIndex(), idx, idx );

    task->setCommonConfig( m_commonConfig );
    task->worker()->setCache( &m_config->db()->cache() );
    m_tasks.insert( idx, task );
    connect( task, SIGNAL(started()),               this, SLOT(onTaskStarted()) );
    connect( task, SIGNAL(progressChanged(double)), this, SLOT(onTaskProgress(double)) );
    connect( task, SIGNAL(finished()),              this, SLOT(onTaskFinished()) );
    connect( task, SIGNAL(dirtyChanged(bool)),      this, SLOT(onTaskDirty(bool)) );

    m_commonTasks->setFinal( m_tasks.back() );
    if( task->config()->name() == "ufraw" )
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
    if( task->config()->name() == "ufraw" )
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

//void TaskStack::saveToFile(QString path)
//{
//    if( m_tasks.empty() )
//        return;

//    QFileInfo rawInfo( m_commonConfig->raw() );
//    qDebug() << "TaskStack::saveToFile()" << rawInfo.absoluteFilePath();

//    QFileInfo cfgInfo;
//    if( path.isEmpty() )
//    {
//        cfgInfo = QFileInfo( rawInfo.dir(), rawInfo.completeBaseName() + ".rawstack" );
//    }
//    else
//    {
//        QFileInfo pathInfo(path);
//        cfgInfo = QFileInfo( pathInfo.dir(), pathInfo.completeBaseName() + ".rawstack" );
//    }

//    setConfig( cfgInfo.absoluteFilePath() );
//    qDebug() << "TaskStack::saveToFile()" << m_config;
//    ConfigFileSaver fileSaver;
//    fileSaver.add( m_commonConfig );
//    std::for_each( m_tasks.begin(), m_tasks.end(), [&] (TaskBase *task) {
//        fileSaver.add( task->config() );
//    });
//    fileSaver.save(m_config);
//}

void TaskStack::loadConfig(ConfigDbEntry *config)
{
    if( m_config == config || (m_config && m_config->equals(config)) )
        return;

    qDebug() << "TaskStack::loadFromFile()" << (config ? config->config() : "unload");

    clearTasks();

    if( m_config )
    {
        disconnect( m_config, SIGNAL(rawChanged(QString)), this, SLOT(onRawChanged()) );
        disconnect( m_config, SIGNAL(destroyed(QObject*)), this, SLOT(onConfigDestroyed()) );
    }
    m_config = config;

    if( m_config )
    {
        connect( m_config, SIGNAL(rawChanged(QString)), this, SLOT(onRawChanged()) );
        connect( m_config, SIGNAL(destroyed(QObject*)), this, SLOT(onConfigDestroyed()) );
        onRawChanged();
        if( !loadTasks() )
            applyDefaultTasks();
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

bool TaskStack::loadTasks()
{
    ConfigFileLoader loader;
    connect( &loader, &ConfigFileLoader::config, [&] (ConfigBase *cfg) {
        qDebug() << "TaskStack::loadTasks()" << cfg;
        if( cfg->name() == "common" )
            setCommonConfig( reinterpret_cast<CommonConfig*>(cfg) );
        else
            addTask( TaskFactory::getInstance()->create(cfg,m_workerThread) );
    });

    bool loaded = loader.load(m_config->config());
    if( !loaded )
    {
        qDebug() << "TaskStack::loadTasks() failed to load" << m_config->config();
    }

    // Check that RAW image really exists,
    // fix RAW directory by current config directory if RAW can't be found.
    QFileInfo cfg(m_config->config());
    QFileInfo raw(m_config->raw());
    if( !raw.exists() )
        raw = QFileInfo(cfg.absoluteDir(),cfg.fileName());
    if( raw.exists() )
        m_config->setRaw( raw.absoluteFilePath() );
    else
        m_config->setRaw( "" );
    return raw.exists();
}

void TaskStack::applyDefaultTasks()
{
    qDebug() << "TaskStack::applyDefaultTasks()" << m_config->raw();

    if( !m_config->isValidRaw() )
    {
        qDebug() << "TaskStack::applyDefaultTasks() unsupported raw";
        return;
    }

    setCommonConfig( new CommonConfig() );
    m_commonConfig->setRaw( m_config->raw() );

    QStringList tasks;
    tasks << "ufraw" << "rotate";
    foreach( QString task, tasks )
    {
        addTask( TaskFactory::getInstance()->create( TaskFactory::getInstance()->create(task), m_workerThread ) );
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

void TaskStack::setCommonConfig(CommonConfig *common)
{
    if( m_commonConfig )
    {
        std::for_each( m_tasks.begin(), m_tasks.end(), [&] (TaskBase *task)
        {
            task->setCommonConfig( NULL );
        });
    }

    delete m_commonConfig;
    common->setParent(this);
    m_commonConfig = common;

    if( m_commonConfig )
    {
        std::for_each( m_tasks.begin(), m_tasks.end(), [&] (TaskBase *task)
        {
            task->setCommonConfig( m_commonConfig );
        });
    }
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
    if( nextIdx >= m_tasks.size() )
    {
        //FIXME: saveConfig();
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
    loadConfig( NULL );
}

void TaskStack::onRawChanged()
{
    if( m_commonConfig && m_config )
        m_commonConfig->setRaw( m_config->raw() );
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
