#ifndef UFRAWTASK_H
#define UFRAWTASK_H

#include "taskbase.h"

class UfrawTask : public TaskBase
{
    Q_OBJECT

public:

    explicit UfrawTask(QObject *parent, QThread *workerThread);
    virtual ~UfrawTask();
};

#endif // UFRAWTASK_H
