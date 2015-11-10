#include "rotatetask.h"
#include "rotateworker.h"
#include "rotatesettings.h"

#include <QDebug>
#include <QThread>

RotateTask::RotateTask(QObject *parent, QThread *workerThread)
    : TaskBase("rotate",parent)
{
    qDebug() << "RotateTask::RotateTask()" << this << workerThread;
    setWorker( new RotateWorker() );
    worker()->moveToThread( workerThread );
}

RotateTask::~RotateTask()
{
    // EMPTY
}

void RotateTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_degree = settings->getSetting(RotateSettings::Degree);
    m_degree->initIfNull( RotateSettings::DefaultDegree );
}
