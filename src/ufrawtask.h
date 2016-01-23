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
#ifndef UFRAWTASK_H
#define UFRAWTASK_H

#include "taskbase.h"

class ConfigSetting;

class UfrawTask : public TaskBase
{
    Q_OBJECT
    Q_PROPERTY(ConfigSetting* fuse          READ fuse          CONSTANT)
    Q_PROPERTY(ConfigSetting* exposuresigma READ exposuresigma CONSTANT)
    Q_PROPERTY(ConfigSetting* exposure      READ exposure      CONSTANT)
    Q_PROPERTY(ConfigSetting* wbTemp        READ wbTemp        CONSTANT)
    Q_PROPERTY(ConfigSetting* wbGreen       READ wbGreen       CONSTANT)

public:

    explicit UfrawTask(QObject *parent, QThread *workerThread);
    virtual ~UfrawTask();

    // TaskBase interface
    virtual bool canDisable() override { return false; }

    ConfigSetting* fuse() { return m_fuse; }
    ConfigSetting* exposuresigma() { return m_exposuresigma; }
    ConfigSetting* exposure() { return m_exposure; }
    ConfigSetting* wbTemp() { return m_wbTemp; }
    ConfigSetting* wbGreen() { return m_wbGreen; }

private:

    // TaskBase interface
    virtual void initTaskSettings() override;

private:

    ConfigSetting* m_fuse;
    ConfigSetting* m_exposuresigma;
    ConfigSetting* m_exposure;
    ConfigSetting* m_wbTemp;
    ConfigSetting* m_wbGreen;
};

#endif // UFRAWTASK_H
