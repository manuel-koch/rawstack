#include "rotatetask.h"
#include "rotateworker.h"
#include "rotateconfig.h"

#include <QDebug>
#include <QThread>

RotateTask::RotateTask(QObject *parent, QThread *workerThread)
    : TaskBase(parent)
{
    qDebug() << "RotateTask::RotateTask()" << this << workerThread;
    setWorker( new RotateWorker() );
    worker()->moveToThread( workerThread );
}

RotateTask::~RotateTask()
{
    // EMPTY
}

