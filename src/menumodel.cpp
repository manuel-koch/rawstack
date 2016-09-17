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
#include "menumodel.h"
#include "menuentry.h"
#include <QDebug>
#include <algorithm>

MenuModel* MenuModel::ms_model = nullptr;

MenuModel::MenuModel(QObject *parent)
    : QObject(parent)
{
    m_screenOverview   = new MenuEntry(this);
    m_screenDevelop    = new MenuEntry(this);

    m_screenGridToggle = new MenuEntry(this);
    m_screenGridFive   = new MenuEntry(this);
    m_screenGridTen    = new MenuEntry(this);
    m_screenGridTwenty = new MenuEntry(this);
    m_screenGridThirds = new MenuEntry(this);
    m_screenGridPhi    = new MenuEntry(this);

    m_screenZoomIn     = new MenuEntry(this);
    m_screenZoomOut    = new MenuEntry(this);
    m_screenZoomFit    = new MenuEntry(this);

    m_screenExposure   = new MenuEntry(this);
}

MenuModel *MenuModel::getInstance()
{
    return ms_model;
}

void MenuModel::setInstance(MenuModel *model)
{
    if( !ms_model )
        ms_model = model;
}
