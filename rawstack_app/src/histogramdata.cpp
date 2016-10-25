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
#include "histogramdata.h"

#include <QDebug>

HistogramData::HistogramData(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

void HistogramData::setRed(const QVariantList &red)
{
    if( red == m_red )
        return;
    m_red = red;
    emit redChanged( m_red );
}

void HistogramData::setGreen(const QVariantList &green)
{
    if( green == m_green )
        return;
    m_green = green;
    emit greenChanged( m_green );
}

void HistogramData::setBlue(const QVariantList &blue)
{
    if( blue == m_blue )
        return;
    m_blue = blue;
    emit blueChanged( m_blue );
}

void HistogramData::setLum(const QVariantList &lum)
{
    if( lum == m_lum )
        return;
    m_lum = lum;
    emit lumChanged( m_lum );
}
