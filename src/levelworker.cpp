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
#include "levelworker.h"
#include "levelsettings.h"
#include "configdbentry.h"
#include "configsetting.h"

#include <math.h>

#include <QDebug>
#include <QThread>

LevelWorker::LevelWorker()
    : WorkerBase(LevelSettings::Name)
{
    // EMPTY
}

LevelWorker::~LevelWorker()
{
    // EMPTY
}

void LevelWorker::registerSettingsImpl()
{
    hashSetting(LevelSettings::Blackpoint);
    hashSetting(LevelSettings::Midpoint);
    hashSetting(LevelSettings::Whitepoint);
    hashSetting(LevelSettings::Brightness);
    hashSetting(LevelSettings::Saturation);
}

void LevelWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    Q_UNUSED(preview);

    qDebug() << "LevelWorker::developImpl()" << this << predecessor;

    double blackpoint = config()->settings()->getSetting(LevelSettings::Blackpoint)->value().toDouble();
    double midpoint   = config()->settings()->getSetting(LevelSettings::Midpoint)->value().toDouble();
    double whitepoint = config()->settings()->getSetting(LevelSettings::Whitepoint)->value().toDouble();
    double brightness = config()->settings()->getSetting(LevelSettings::Brightness)->value().toDouble();
    double saturation = config()->settings()->getSetting(LevelSettings::Saturation)->value().toDouble();

    if( blackpoint != LevelSettings::DefaultBlackpoint ||
        midpoint   != LevelSettings::DefaultMidpoint   ||
        whitepoint != LevelSettings::DefaultWhitepoint    )
    {
        qDebug() << "LevelWorker::developImpl() level" << this << blackpoint << midpoint << whitepoint;
        double b = blackpoint*MaxRGB;
        double m = std::min( std::max( midpoint, 0.1 ), 10.0 );
        double w = whitepoint*MaxRGB;
        m_img.level( b, w, m );
        setProgress(0.5);
    }

    if( brightness != LevelSettings::DefaultBrightness ||
        saturation != LevelSettings::DefaultSaturation    )
    {
        qDebug() << "LevelWorker::developImpl() modulate" << this << brightness << saturation;
        m_img.modulate( brightness*100, saturation*100, 100 );
    }
}
