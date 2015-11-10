#include "enfuseprocess.h"

#include <QDebug>
#include <QRegularExpression>

EnfuseProcess::EnfuseProcess(QObject *parent)
    : QProcess(parent)
    , m_output("XXXXXX.tif")
{
    connect( this, SIGNAL(finished(int)),                        this, SLOT(onTerminated(int)) );
    connect( this, SIGNAL(started()),                            this, SLOT(onStarted()) );
    connect( this, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onStateChanged(QProcess::ProcessState)) );
    connect( this, SIGNAL(error(QProcess::ProcessError)),        this, SLOT(onError(QProcess::ProcessError)) );
    connect( this, SIGNAL(readyReadStandardOutput()),            this, SLOT(onConsole()) );
}

EnfuseProcess::~EnfuseProcess()
{
    if( state() != QProcess::Running )
        waitForFinished();
}

void EnfuseProcess::run()
{
    if( state() == QProcess::Running )
        return;
    qDebug() << "EnfuseProcess::run() fuse...";

    m_output.open();
    m_console.clear();

    QStringList args(m_inputs);
    args << "-v" << "--depth=16"
         << "--exposure-weight=1"
         << "--contrast-weight=0"
         << "--saturation-weight=0"
         << "--gray-projector=luminance"
         << "-o" << m_output.fileName();
    qDebug() << "EnfuseProcess::run()" << args;
    setArguments( args );
    setProcessChannelMode( QProcess::MergedChannels );
    start();
}

void EnfuseProcess::onTerminated(int exitCode)
{
    qDebug() << "EnfuseProcess::onTerminated()" << exitCode;
}

void EnfuseProcess::onStarted()
{
    qDebug() << "EnfuseProcess::onStarted() pid" << pid();
}

void EnfuseProcess::onStateChanged(QProcess::ProcessState newState)
{
    qDebug() << "EnfuseProcess::onStateChanged()" << newState;
}

void EnfuseProcess::onError(QProcess::ProcessError error)
{
    qDebug() << "EnfuseProcess::onError()" << error;
}

void EnfuseProcess::onConsole()
{
    QByteArray d = readAllStandardOutput();
    m_console.append( d );

    int nofLoaded     = getNofLoaded();
    int nofTotalLevel = 16;
    int nofLevel      = getNofLevels();

    double loaded = (double)nofLoaded / m_inputs.size();
    double levels = 0;
    if( nofTotalLevel )
        levels = (double)nofLevel / nofTotalLevel;

    emit progress( (loaded + levels) / 2 );
}

int EnfuseProcess::getNofLoaded()
{
    int nofLoaded = 0;
    QRegularExpression loadedRe("loading next image");
    QRegularExpressionMatchIterator i = loadedRe.globalMatch( m_console );
    while( i.hasNext() )
    {
        i.next();
        nofLoaded++;
    }
    qDebug() << "EnfuseProcess::getNofLoaded()" << nofLoaded;
    return nofLoaded;
}

int EnfuseProcess::getTotalLevels()
{
    QRegularExpression nofLevelRe("using (?P<nof>\\d+) blending level");
    QRegularExpressionMatch m = nofLevelRe.match( m_console );
    int nofLevel = 0;
    if( m.hasMatch() )
        nofLevel = m.captured("nof").toInt();
    qDebug() << "EnfuseProcess::getTotalLevels()" << nofLevel;
    return nofLevel;
}

int EnfuseProcess::getNofLevels()
{
    int nofLevel = 0;
    QRegularExpression levelRe("(generating|collapsing) \\w+ pyramid");
    QRegularExpressionMatchIterator i = levelRe.globalMatch( m_console );
    while( i.hasNext() )
    {
        i.next();
        nofLevel++;
    }
    qDebug() << "EnfuseProcess::getNofLevels()" << nofLevel;
    return nofLevel;
}
