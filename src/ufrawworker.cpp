#include "ufrawworker.h"
#include "ufrawprocess.h"
#include "ufrawconfig.h"

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

    UfrawConfig *cfg = config<UfrawConfig>();

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( "/Users/manuel/tmp/TestBilder/01.cr2" );
    ufraw.setExposure( cfg->exposure() );
    ufraw.setShrink( 4 );
    ufraw.run();
    ufraw.waitForStarted(-1);
    setProgress(0.1);
    ufraw.waitForFinished(-1);
    setProgress(0.8);

    qDebug() << "UfrawWorker::run() finished" << QString::fromUtf8( ufraw.err() );
    if( ufraw.exitCode() == 0 )
    {
        qDebug() << "UfrawWorker::run() data" << ufraw.out().size();
        Magick::Blob blob( ufraw.out().constData(), ufraw.out().size() );
        m_img.read( blob );
    }
}

