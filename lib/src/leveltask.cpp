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
#include "leveltask.h"
#include "levelworker.h"
#include "levelsettings.h"

#include <QDebug>
#include <QThread>

LevelTask::LevelTask(QObject *parent, QThread *workerThread)
    : TaskBase(LevelSettings::Name,parent)
{
    qDebug() << "LevelTask::LevelTask()" << this << workerThread;
    setWorker( new LevelWorker() );
    worker()->moveToThread( workerThread );
}

LevelTask::~LevelTask()
{
    // EMPTY
}

void LevelTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_blackpoint = settings->getSetting( LevelSettings::Blackpoint );
    m_blackpoint->initIfNull( LevelSettings::DefaultBlackpoint );

    m_midpoint = settings->getSetting( LevelSettings::Midpoint );
    m_midpoint->initIfNull( LevelSettings::DefaultMidpoint );

    m_whitepoint = settings->getSetting( LevelSettings::Whitepoint );
    m_whitepoint->initIfNull( LevelSettings::DefaultWhitepoint );

    m_brightness = settings->getSetting( LevelSettings::Brightness );
    m_brightness->initIfNull( LevelSettings::DefaultBrightness );

    m_saturation = settings->getSetting( LevelSettings::Saturation );
    m_saturation->initIfNull( LevelSettings::DefaultSaturation );
}
