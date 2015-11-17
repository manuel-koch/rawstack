#include "sharpentask.h"
#include "sharpenworker.h"
#include "sharpensettings.h"

#include <QDebug>
#include <QThread>

SharpenTask::SharpenTask(QObject *parent, QThread *workerThread)
    : TaskBase("sharpen",parent)
{
    qDebug() << "SharpenTask::SharpenTask()" << this << workerThread;
    setWorker( new SharpenWorker() );
    worker()->moveToThread( workerThread );
}

SharpenTask::~SharpenTask()
{
    // EMPTY
}

void SharpenTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_radius = settings->getSetting( SharpenSettings::Radius );
    m_radius->initIfNull( SharpenSettings::DefaultRadius );

    m_sigma = settings->getSetting( SharpenSettings::Sigma );
    m_sigma->initIfNull( SharpenSettings::DefaultSigma );
}
