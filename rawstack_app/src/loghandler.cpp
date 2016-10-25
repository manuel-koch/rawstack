/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
        ts.flush();
    }

    fprintf(stderr, "%s\n",txt.toStdString().c_str());
    fflush(stderr);
}

