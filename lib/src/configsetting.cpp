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
#include "configsetting.h"

#include <QDebug>

ConfigSetting::ConfigSetting(QString fullname, QObject *parent)
    : QObject(parent)
    , m_fullname(fullname)
{
}

QString ConfigSetting::task() const
{
    QStringList parts = m_fullname.split(".");
    Q_ASSERT(parts.size()>1);
    if( parts.isEmpty() )
        return m_fullname;
    else
        return parts.front();
}

QString ConfigSetting::group() const
{
    QStringList parts = m_fullname.split(".");
    Q_ASSERT(parts.size()>=2);
    QString group;
    for( int i=1; i<parts.size()-1; i++ )
        group += (group.isEmpty() ? "" : "." ) + parts[i];
    return group;
}

QString ConfigSetting::name() const
{
    QStringList parts = m_fullname.split(".");
    Q_ASSERT(parts.size()>=2);
    return parts.back();
}

void ConfigSetting::setValue(QVariant value)
{
    if( m_value == value )
        return;

    m_value = value;
    qDebug() << "ConfigSetting::setValue()" << m_fullname << m_value;
    emit valueChanged(m_value);
}

