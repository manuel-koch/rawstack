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
#include "ufrawtask.h"
#include "ufrawworker.h"
#include "ufrawsettings.h"
#include "configsetting.h"

#include <QDebug>
#include <QThread>

UfrawTask::UfrawTask(QObject *parent, QThread *workerThread)
    : TaskBase("ufraw",parent)
    , m_fuse(NULL)
    , m_exposure(NULL)
    , m_wbTemp(NULL)
    , m_wbGreen(NULL)
{
    qDebug() << "UfrawTask::UfrawTask()" << this << workerThread;
    setWorker( new UfrawWorker() );
    worker()->moveToThread( workerThread );
}

UfrawTask::~UfrawTask()
{
    // EMPTY
}

void UfrawTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_fuse = settings->getSetting(UfrawSettings::Fuse);
    m_fuse->initIfNull( UfrawSettings::DefaultFuse );

    m_exposuresigma = settings->getSetting(UfrawSettings::ExposureSigma);
    m_exposuresigma->initIfNull( UfrawSettings::DefaultExposureSigma );

    m_exposure = settings->getSetting(UfrawSettings::Exposure);
    m_exposure->initIfNull( UfrawSettings::DefaultExposure );

    m_wbTemp = settings->getSetting(UfrawSettings::WbTemperature);
    m_wbTemp->initIfNull( UfrawSettings::DefaultWbTemperature );

    m_wbGreen = settings->getSetting(UfrawSettings::WbGreen);
    m_wbGreen->initIfNull( UfrawSettings::DefaultWbGreen );
}
