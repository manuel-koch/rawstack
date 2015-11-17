#ifndef SHARPENTASK_H
#define SHARPENTASK_H

#include "taskbase.h"
#include "configsetting.h"

class SharpenTask : public TaskBase
{
    Q_OBJECT
    Q_PROPERTY(ConfigSetting* radius READ radius CONSTANT)
    Q_PROPERTY(ConfigSetting* sigma  READ sigma  CONSTANT)

public:

    explicit SharpenTask(QObject *parent, QThread *workerThread);
    virtual ~SharpenTask();

    ConfigSetting* radius() { return m_radius; }
    ConfigSetting* sigma() { return m_sigma; }

private:

    // TaskBase interface
    virtual void initTaskSettings();

private:

    ConfigSetting* m_radius;
    ConfigSetting* m_sigma;
};

#endif // SHARPENTASK_H
