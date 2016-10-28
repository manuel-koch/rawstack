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
#include "menuentry.h"

MenuEntry::MenuEntry(QObject *parent)
    : QObject(parent)
    , m_title()
    , m_enabled(true)
    , m_checkable(false)
    , m_checked(false)
{
    // EMPTY
}

MenuEntry::~MenuEntry()
{
    // EMPTY
}

void MenuEntry::setTitle(QString title)
{
    if( m_title == title )
        return;

    m_title = title;
    emit titleChanged(title);
}

void MenuEntry::setEnabled(bool enabled)
{
    if( m_enabled == enabled )
        return;

    m_enabled = enabled;
    emit enabledChanged(enabled);
}

void MenuEntry::setCheckable(bool checkable)
{
    if( m_checkable == checkable )
        return;

    m_checkable = checkable;
    emit checkableChanged(checkable);
}

void MenuEntry::setChecked(bool checked)
{
    if( m_checked == checked )
        return;

    m_checked = checked;
    emit checkedChanged(checked);
}
