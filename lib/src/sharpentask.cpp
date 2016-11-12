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
#include "sharpentask.h"
#include "sharpenworker.h"
#include "sharpensettings.h"

#include <QDebug>
#include <QThread>

SharpenTask::SharpenTask(QObject *parent, QThread *workerThread)
    : TaskBase("sharpen",parent)
{
    qDebug() << "SharpenTask::SharpenTask()" << this << workerThread;
    setWorker( new SharpenWorker() );
    worker()->moveToThread( workerThread );
}

SharpenTask::~SharpenTask()
{
    // EMPTY
}

void SharpenTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_radius = settings->getSetting( SharpenSettings::Radius );
    m_radius->initIfNull( SharpenSettings::DefaultRadius );

    m_sigma = settings->getSetting( SharpenSettings::Sigma );
    m_sigma->initIfNull( SharpenSettings::DefaultSigma );

    m_amount = settings->getSetting( SharpenSettings::Amount );
    m_amount->initIfNull( SharpenSettings::DefaultAmount );

    m_threshold = settings->getSetting( SharpenSettings::Threshold );
    m_threshold->initIfNull( SharpenSettings::DefaultThreshold );
}
