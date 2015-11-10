#include "ufrawtask.h"
#include "ufrawworker.h"
#include "ufrawsettings.h"
#include "configsetting.h"

#include <QDebug>
#include <QThread>

UfrawTask::UfrawTask(QObject *parent, QThread *workerThread)
    : TaskBase("ufraw",parent)
    , m_fuse(NULL)
    , m_exposure(NULL)
    , m_wbTemp(NULL)
    , m_wbGreen(NULL)
{
    qDebug() << "UfrawTask::UfrawTask()" << this << workerThread;
    setWorker( new UfrawWorker() );
    worker()->moveToThread( workerThread );
}

UfrawTask::~UfrawTask()
{
    // EMPTY
}

void UfrawTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_fuse = settings->getSetting(UfrawSettings::Fuse);
    m_fuse->initIfNull( UfrawSettings::DefaultFuse );

    m_exposure = settings->getSetting(UfrawSettings::Exposure);
    m_exposure->initIfNull( UfrawSettings::DefaultExposure );

    m_wbTemp = settings->getSetting(UfrawSettings::WbTemperature);
    m_wbTemp->initIfNull( UfrawSettings::DefaultWbTemperature );

    m_wbGreen = settings->getSetting(UfrawSettings::WbGreen);
    m_wbGreen->initIfNull( UfrawSettings::DefaultWbGreen );
}
