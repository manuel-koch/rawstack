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
#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QDomNode>

class ConfigSetting;

class ConfigSettings : public QObject
{
    Q_OBJECT

public:

    explicit ConfigSettings( QObject *parent = NULL );
    virtual ~ConfigSettings();

    /// Return list of tasks that have a setting
    QStringList tasks() const;

    /// Return list of all settings or those starting with selected prefix ( task/group )
    QStringList settings(QString prefix = "") const;

    /// Return a named setting, creates a new setting if it doesn't exist yet
    ConfigSetting *getSetting(QString fullname);

    /// Return a setting by index
    ConfigSetting *getSetting(int index) const;

    /// Remove fully named setting or all settings of named task
    void removeSetting(QString name);

    /// Remove all settings
    void removeAll();

    /// Convert settings to XML by adding element hierarchy to given DOM node
    void toXML(QDomNode &node) const;

    /// Init settings from XML by parsing element hierarchy from given DOM node
    void fromXML(const QDomNode &node);

    /// copy settings from other instance
    void operator =(const ConfigSettings &other);

private:

    ConfigSetting *appendNewSetting(QString fullname);

    QList<ConfigSetting*>         m_settings;
    QHash<QString,ConfigSetting*> m_map;
};

#endif // CONFIGSETTINGS_H
