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

