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
#include "taskhistogram.h"
#include "histogrammaker.h"
#include "taskbase.h"

#include <QDebug>

TaskHistogram::TaskHistogram(QObject *parent)
    : HistogramData(parent)
    , m_task(NULL)
{
    // EMPTY
}

void TaskHistogram::setTask(TaskBase *task)
{
    if( m_task == task )
        return;

    qDebug() << "TaskHistogram::setTask()" << task;

    if( m_task )
    {
        disconnect(m_cycleConn);
        disconnect(m_redConn);
        disconnect(m_greenConn);
        disconnect(m_blueConn);
        disconnect(m_lumConn);
    }

    m_task = task;

    if( m_task )
    {
        m_cycleConn = connect( m_task->worker(), &WorkerBase::cycleChanged,
                               m_task->worker(), &WorkerBase::requestHistogram );
        m_redConn   = connect( m_task->worker()->histogram(), &HistogramMaker::redChanged,
                               this, &TaskHistogram::setRed );
        m_greenConn = connect( m_task->worker()->histogram(), &HistogramMaker::greenChanged,
                               this, &TaskHistogram::setGreen );
        m_blueConn  = connect( m_task->worker()->histogram(), &HistogramMaker::blueChanged,
                               this, &TaskHistogram::setBlue );
        m_lumConn   = connect( m_task->worker()->histogram(), &HistogramMaker::lumChanged,
                               this, &TaskHistogram::setLum );
    }

    emit taskChanged(m_task);
}

