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
#ifndef SHARPENTASK_H
#define SHARPENTASK_H

#include "taskbase.h"
#include "configsetting.h"

class SharpenTask : public TaskBase
{
    Q_OBJECT
    Q_PROPERTY(ConfigSetting* radius    READ radius    CONSTANT)
    Q_PROPERTY(ConfigSetting* sigma     READ sigma     CONSTANT)
    Q_PROPERTY(ConfigSetting* amount    READ amount    CONSTANT)
    Q_PROPERTY(ConfigSetting* threshold READ threshold CONSTANT)

public:

    explicit SharpenTask(QObject *parent, QThread *workerThread);
    virtual ~SharpenTask();

    ConfigSetting* radius() { return m_radius; }
    ConfigSetting* sigma() { return m_sigma; }
    ConfigSetting* amount() { return m_amount; }
    ConfigSetting* threshold() { return m_threshold; }

private:

    // TaskBase interface
    virtual void initTaskSettings();

private:

    ConfigSetting* m_radius;
    ConfigSetting* m_sigma;
    ConfigSetting* m_amount;
    ConfigSetting* m_threshold;
};

#endif // SHARPENTASK_H
