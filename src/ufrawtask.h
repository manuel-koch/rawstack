#ifndef UFRAWTASK_H
#define UFRAWTASK_H

#include "taskbase.h"

class ConfigSetting;

class UfrawTask : public TaskBase
{
    Q_OBJECT
    Q_PROPERTY(ConfigSetting* fuse     READ fuse     CONSTANT)
    Q_PROPERTY(ConfigSetting* exposure READ exposure CONSTANT)
    Q_PROPERTY(ConfigSetting* wbTemp   READ wbTemp   CONSTANT)
    Q_PROPERTY(ConfigSetting* wbGreen  READ wbGreen  CONSTANT)

public:

    explicit UfrawTask(QObject *parent, QThread *workerThread);
    virtual ~UfrawTask();

    // TaskBase interface
    virtual bool canDisable() override { return false; }

    ConfigSetting* fuse() { return m_fuse; }
    ConfigSetting* exposure() { return m_exposure; }
    ConfigSetting* wbTemp() { return m_wbTemp; }
    ConfigSetting* wbGreen() { return m_wbGreen; }

private:

    // TaskBase interface
    virtual void initTaskSettings() override;

private:

    ConfigSetting* m_fuse;
    ConfigSetting* m_exposure;
    ConfigSetting* m_wbTemp;
    ConfigSetting* m_wbGreen;
};

#endif // UFRAWTASK_H
