#include "ufrawworker.h"
#include "ufrawprocess.h"
#include "ufrawconfig.h"

#include <QDebug>
#include <QThread>

UfrawWorker::UfrawWorker()
    : WorkerBase()
{
    // EMPTY
}

UfrawWorker::~UfrawWorker()
{
    // EMPTY
}

void UfrawWorker::prepareImpl()
{
    qDebug() << "UfrawWorker::prepareImpl()" << this;

    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg || cfg->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::prepareImpl() invalid config";
        return;
    }

    if( cfg->wbTemperature() != UfrawConfig::DefaultWbTemperature &&
        cfg->wbGreen()       != UfrawConfig::DefaultWbGreen )
        return;

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( cfg->raw() );
    ufraw.setExposure( cfg->exposure() );
    ufraw.setShrink( 4 );

    // probe for other settings first...
    ufraw.run( true );
    if( cfg->wbTemperature() == UfrawConfig::DefaultWbTemperature )
        cfg->setWbTemperature( ufraw.wbTemperature() );
    if( cfg->wbGreen() == UfrawConfig::DefaultWbGreen )
        cfg->setWbGreen( ufraw.wbGreen() );
}

void UfrawWorker::developImpl(WorkerBase *predecessor)
{
    qDebug() << "UfrawWorker::developImpl()" << this << predecessor;

    UfrawConfig *cfg = config<UfrawConfig>();
    if( !cfg || cfg->raw().isEmpty() )
    {
        qWarning() << "UfrawWorker::developImpl() invalid config";
        return;
    }

    UfrawProcess ufraw;
    ufraw.setProgram( "/opt/local/bin/ufraw-batch" );
    ufraw.setRaw( cfg->raw() );
    ufraw.setExposure( cfg->exposure() );
    ufraw.setShrink( 4 );
    ufraw.setWbTemperature( cfg->wbTemperature() );
    ufraw.setWbGreen( cfg->wbGreen() );

    // extract the image
    ufraw.run( false );
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

