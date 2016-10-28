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
#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>
#include <QDebug>
#include <QtDebug>
#include <QFile>
#include <QFile>

class LogHandler : public QObject
{
    Q_OBJECT

public:

    explicit LogHandler(QObject *parent = 0);
    virtual ~LogHandler();

public slots:

    void start( const QString &path );
    void stop();

private:

    static void setInstance( LogHandler *instance ) { ms_instance = instance; }
    static void onLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    void onLogImpl(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:

    static LogHandler *ms_instance;
    QFile m_log;
};

#endif // LOGHANDLER_H
