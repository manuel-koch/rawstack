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
#ifndef CONFIGSETTINGSSTORE_H
#define CONFIGSETTINGSSTORE_H

#include "configdbentry.h"
#include "configsettings.h"

#include <QAbstractListModel>
#include <QList>

class ConfigSettingsStore : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int nofSettings         READ nofSettings         NOTIFY nofSettingsChanged)
    Q_PROPERTY(int nofSelectedSettings READ nofSelectedSettings NOTIFY nofSelectedSettingsChanged)
    Q_ENUMS(Roles)

public:

    typedef QHash<int, QByteArray> RoleMap;

    enum Roles
    {
        NameRole = Qt::UserRole+1,
        ValueRole,
        SelectedRole,
    };

    explicit ConfigSettingsStore(QObject *parent = 0);
    virtual ~ConfigSettingsStore();

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

    int nofSettings() const { return m_nofSettings; }
    int nofSelectedSettings() const { return m_nofSelectedSettings; }

signals:

    void nofSettingsChanged(int nofSettings);
    void nofSelectedSettingsChanged(int nofSelectedSettings);

public slots:

    void copyFrom(ConfigDbEntry *config);
    void applyTo(ConfigDbEntry *config);

private:

    void setNofSettings(int nof);
    void setNofSelectedSettings(int nof);

private:

    RoleMap         m_rolemap;
    int             m_nofSettings;
    int             m_nofSelectedSettings;
    QList<bool>     m_selected;
    ConfigSettings  m_settings;
};

#endif // CONFIGSETTINGSSTORE_H
