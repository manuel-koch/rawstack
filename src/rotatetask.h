#ifndef ROTATETASK_H
#define ROTATETASK_H

#include "taskbase.h"
#include "configsetting.h"

class RotateTask : public TaskBase
{
    Q_OBJECT
    Q_PROPERTY(ConfigSetting* degree  READ degree  CONSTANT)

public:

    explicit RotateTask(QObject *parent, QThread *workerThread);
    virtual ~RotateTask();

    ConfigSetting* degree() { return m_degree; }

private:

    // TaskBase interface
    virtual void initTaskSettings();

private:

    ConfigSetting* m_degree;
};

#endif // ROTATETASK_H
