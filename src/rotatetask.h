#ifndef ROTATETASK_H
#define ROTATETASK_H

#include "taskbase.h"

class RotateTask : public TaskBase
{
    Q_OBJECT

public:

    explicit RotateTask(QObject *parent, QThread *workerThread);
    virtual ~RotateTask();
};

#endif // ROTATETASK_H
