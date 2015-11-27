#ifndef LEVELTASK_H
#define LEVELTASK_H

#include "taskbase.h"
#include "configsetting.h"

class LevelTask : public TaskBase
{
    Q_OBJECT
    Q_PROPERTY(ConfigSetting* blackpoint READ blackpoint CONSTANT)
    Q_PROPERTY(ConfigSetting* midpoint   READ midpoint   CONSTANT)
    Q_PROPERTY(ConfigSetting* whitepoint READ whitepoint CONSTANT)
    Q_PROPERTY(ConfigSetting* brightness READ brightness CONSTANT)
    Q_PROPERTY(ConfigSetting* saturation READ saturation CONSTANT)

public:

    explicit LevelTask(QObject *parent, QThread *workerThread);
    virtual ~LevelTask();

    ConfigSetting* blackpoint() { return m_blackpoint; }
    ConfigSetting* midpoint() { return m_midpoint; }
    ConfigSetting* whitepoint() { return m_whitepoint; }
    ConfigSetting* brightness() { return m_brightness; }
    ConfigSetting* saturation() { return m_saturation; }

private:

    // TaskBase interface
    virtual void initTaskSettings();

private:

    ConfigSetting* m_blackpoint;
    ConfigSetting* m_midpoint;
    ConfigSetting* m_whitepoint;
    ConfigSetting* m_brightness;
    ConfigSetting* m_saturation;
};

#endif // LEVELTASK_H
