#include "ufrawtask.h"
#include "ufrawworker.h"
#include "ufrawconfig.h"

#include <QDebug>
#include <QThread>

UfrawTask::UfrawTask(QObject *parent, QThread *workerThread)
    : TaskBase(parent)
{
    qDebug() << "UfrawTask::UfrawTask()" << this << workerThread;
    setWorker( new UfrawWorker() );
    worker()->moveToThread( workerThread );
}

UfrawTask::~UfrawTask()
{
    // EMPTY
}

