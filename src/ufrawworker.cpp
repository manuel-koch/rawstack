#include "ufrawworker.h"

#include <QDebug>
#include <QThread>

UfrawWorker::UfrawWorker(ConfigBase *config)
    : WorkerBase(config)
{
    // EMPTY
}

UfrawWorker::~UfrawWorker()
{
    // EMPTY
}

void UfrawWorker::developImpl(WorkerBase *predecessor)
{
    qDebug() << "UfrawWorker::developImpl()" << this << predecessor;
    QThread::sleep(5);
    setProgress(0.3);
    QThread::sleep(2);
    setProgress(0.8);
    QThread::sleep(2);
}

