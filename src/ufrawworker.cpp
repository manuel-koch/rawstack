#include "ufrawworker.h"
#include "ufrawprocess.h"

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

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( "/Users/manuel/tmp/TestBilder/01.cr2" );
    ufraw.run();
    ufraw.waitForFinished(-1);
}

