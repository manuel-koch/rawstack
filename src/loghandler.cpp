#include "loghandler.h"

#include <qapplication.h>
#include <QTextStream>

LogHandler *LogHandler::ms_instance = NULL;

LogHandler::LogHandler(QObject *parent)
    : QObject(parent)
{
    setInstance( this );
    qInstallMessageHandler( LogHandler::onLog );
}

LogHandler::~LogHandler()
{
    stop();
}

void LogHandler::start(const QString &path)
{
    stop();
    m_log.setFileName( path );
    m_log.open( QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered );
}

void LogHandler::stop()
{
    m_log.close();
}

void LogHandler::onLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if( ms_instance )
        ms_instance->onLogImpl(type,context,msg);
}

void LogHandler::onLogImpl(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt = qFormatLogMessage(type,context,msg);

    if( m_log.isOpen() )
    {
        QTextStream ts(&m_log);
        ts << txt << endl;
    }

    fprintf(stderr, "%s\n",txt.toStdString().c_str());
}

