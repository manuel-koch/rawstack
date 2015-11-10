#include "taskbase.h"
#include "workerbase.h"
#include "configsetting.h"

#include <QDebug>

TaskBase::TaskBase(QString name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_config(NULL)
    , m_worker(NULL)
    , m_images(NULL)
{
    connect( this, SIGNAL(started()),  this, SLOT(onStarted()) );
    connect( this, SIGNAL(finished()), this, SLOT(onFinished()) );
}

TaskBase::~TaskBase()
{
    qDebug() << "TaskBase::~TaskBase()" << this;
    delete m_worker;
}

bool TaskBase::enabled()
{
    ConfigSetting *enabled = m_config->settings()->getSetting(m_name+".enabled");
    return enabled->value().toBool();
}

void TaskBase::setConfig(ConfigDbEntry *config)
{
    qDebug() << "TaskBase::setConfig()" << this << config;
    m_config = config;
    initBaseSettings();
    initTaskSettings();
    m_worker->setConfig(m_config);
}

void TaskBase::setWorker(WorkerBase *worker)
{
    m_worker = worker;
    connect( m_worker, SIGNAL(started()),               this, SIGNAL(started()) );
    connect( m_worker, SIGNAL(finished()),              this, SIGNAL(finished()) );
    connect( m_worker, SIGNAL(progressChanged(double)), this, SIGNAL(progressChanged(double)) );
    connect( m_worker, SIGNAL(dirtyChanged(bool)),      this, SIGNAL(dirtyChanged(bool)) );
    m_images = new ImageFactory(worker,this);
}

void TaskBase::setEnabled(bool enabled)
{
    ConfigSetting *en = m_config->settings()->getSetting(m_name+".enabled");
    en->setValue( enabled );
}

void TaskBase::develop(bool preview, TaskBase *predecessor)
{
    qDebug() << "TaskBase::develop()" << this << predecessor;
    if( m_worker )
    {
       WorkerBase *predecWorker = predecessor ? predecessor->worker() : NULL;
       m_worker->develop( preview, predecWorker );
    }
}

void TaskBase::initBaseSettings()
{
    qDebug() << "TaskBase::initBaseSettings()" << this;
    ConfigSetting *enabled = m_config->settings()->getSetting(m_name+".enabled");
    enabled->initIfNull( true );
    connect( enabled, SIGNAL(valueChanged(QVariant)), this, SLOT(onEnabledChanged(QVariant)) );
    if( !canDisable() && !enabled->value().toBool() )
        enabled->setValue( true );
}

void TaskBase::onEnabledChanged(QVariant enabled)
{
    emit enabledChanged( enabled.toBool() );
}

void TaskBase::initTaskSettings()
{
    qDebug() << "TaskBase::initTaskSettings()" << this;
}

void TaskBase::onStarted()
{
    qDebug() << "TaskBase::onStarted()" << this;
}

void TaskBase::onFinished()
{
    qDebug() << "TaskBase::onFinished()" << this;
}
