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
