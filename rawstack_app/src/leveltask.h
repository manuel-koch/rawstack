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
