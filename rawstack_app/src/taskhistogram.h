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
#ifndef TASKHISTOGRAM_H
#define TASKHISTOGRAM_H

#include "histogramdata.h"

#include <QObject>

class TaskBase;

class TaskHistogram : public HistogramData
{
    Q_OBJECT
    Q_PROPERTY(TaskBase* task READ task WRITE setTask NOTIFY taskChanged)

public:

    explicit TaskHistogram(QObject *parent = NULL);

    TaskBase* task() const { return m_task; }

public slots:

    void setTask(TaskBase* task);

signals:

    void taskChanged(TaskBase* task);

public:

    TaskBase*               m_task;
    QMetaObject::Connection m_cycleConn;
    QMetaObject::Connection m_redConn;
    QMetaObject::Connection m_greenConn;
    QMetaObject::Connection m_blueConn;
    QMetaObject::Connection m_lumConn;
};

#endif // TASKHISTOGRAM_H
