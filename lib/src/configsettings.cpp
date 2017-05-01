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
#include "configsettings.h"
#include "configsetting.h"

#include <QDebug>
#include <QList>

ConfigSettings::ConfigSettings(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

ConfigSettings::~ConfigSettings()
{
    // EMPTY
}

QStringList ConfigSettings::tasks() const
{
    QStringList taskList;
    foreach( ConfigSetting *setting, m_settings )
    {
        QString task = setting->task();
        if( !taskList.contains(task) )
            taskList.append(task);
    }
    return taskList;
}

bool ConfigSettings::hasSettings() const
{
    return !m_settings.empty();
}

QStringList ConfigSettings::settings(QString prefix) const
{
    QStringList settings;

    if( !prefix.isEmpty() )
        prefix += ".";

    foreach( ConfigSetting *setting, m_settings )
    {
        QString fullname = setting->fullname();
        if( fullname.startsWith(prefix) )
            settings.append(fullname);
    }

    return settings;
}

ConfigSetting *ConfigSettings::getSetting(QString fullname)
{
    if( fullname.trimmed().isEmpty() )
        return nullptr;

    if( m_map.contains(fullname) )
        return m_map[fullname];

    return appendNewSetting(fullname);
}

ConfigSetting *ConfigSettings::getSetting(int index) const
{
    if( index < 0 || index >= m_settings.size() )
        return nullptr;

    return m_settings.at(index);
}

void ConfigSettings::removeSetting(QString name)
{
    const bool hadSettings = hasSettings();

    QList<int> indices;
    bool wildcard = !name.contains(".");
    for( int i=0; i<m_settings.size(); i++ )
    {
        if( m_settings[i]->fullname() == name || (wildcard && m_settings[i]->task() == name) )
            indices.insert(0,i);
    }
    foreach( int i, indices )
    {
        ConfigSetting *setting = m_settings.takeAt( i );
        m_map.remove( setting->fullname() );
        delete setting;
    }

    if( hadSettings != hasSettings() )
        emit hasSettingsChanged( !hadSettings );
}

void ConfigSettings::removeAll()
{
    const bool hadSettings = hasSettings();
    while( m_settings.size() )
    {
        ConfigSetting *setting = m_settings.takeAt(0);
        m_map.remove( setting->fullname() );
        delete setting;
    }
    if( hadSettings != hasSettings() )
        emit hasSettingsChanged( !hadSettings );
}

void ConfigSettings::toXML(QDomNode &node) const
{
    QDomDocument doc = node.ownerDocument();
    foreach( ConfigSetting *setting, m_settings )
    {
        QDomNode settingNode = node.appendChild( doc.createElement("setting") );
        QDomElement settingElm = settingNode.toElement();
        settingElm.setAttribute("name",setting->fullname());
        QDomText settingText = doc.createTextNode( setting->value().toString() );
        settingElm.appendChild( settingText );
    }
}

void ConfigSettings::fromXML(const QDomNode &node)
{
    removeAll();
    QDomNodeList settings = node.toElement().elementsByTagName("setting");
    for( int i=0; i<settings.size(); i++ )
    {
        QDomElement settingElm = settings.item(i).toElement();
        QString name = settingElm.attribute("name");
        ConfigSetting *setting = getSetting(name);
        setting->setValue( settingElm.text() );
    }
}

void ConfigSettings::operator =(const ConfigSettings &other)
{
    removeAll();
    foreach( ConfigSetting *otherSetting, other.m_settings )
    {
        ConfigSetting *s = appendNewSetting(otherSetting->fullname());
        s->setValue( otherSetting->value() );
    }
}

ConfigSetting *ConfigSettings::appendNewSetting(QString fullname)
{
    const bool hadSettings = hasSettings();

    ConfigSetting *setting = new ConfigSetting(fullname,this);
    m_settings.append( setting );
    m_map[fullname] = setting;

    if( hadSettings != hasSettings() )
        emit hasSettingsChanged( !hadSettings );

    return setting;
}
