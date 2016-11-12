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
#include "sharpenworker.h"
#include "sharpensettings.h"
#include "configdbentry.h"
#include "configsetting.h"

#include <math.h>

#include <QDebug>
#include <QThread>

SharpenWorker::SharpenWorker()
    : WorkerBase("sharpen")
{
    // EMPTY
}

SharpenWorker::~SharpenWorker()
{
    // EMPTY
}

void SharpenWorker::registerSettingsImpl()
{
    hashSetting(SharpenSettings::Radius);
    hashSetting(SharpenSettings::Sigma);
    hashSetting(SharpenSettings::Amount);
    hashSetting(SharpenSettings::Threshold);
}

void SharpenWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    Q_UNUSED(preview);

    qDebug() << "SharpenWorker::developImpl()" << this << predecessor;

    double radius    = config()->settings()->getSetting(SharpenSettings::Radius)->value().toDouble();
    double sigma     = config()->settings()->getSetting(SharpenSettings::Sigma)->value().toDouble();
    double amount    = config()->settings()->getSetting(SharpenSettings::Amount)->value().toDouble();
    double threshold = config()->settings()->getSetting(SharpenSettings::Threshold)->value().toDouble();
    if( radius>=0 && sigma>0 && amount>0 && threshold>=0 )
    {
        qDebug() << "SharpenWorker::developImpl()" << this << radius << sigma << amount << threshold;
        m_img.unsharpmask( radius, sigma, amount, threshold );
    }
}
