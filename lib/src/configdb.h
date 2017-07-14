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
#ifndef CONFIGDB_H_7FFC7863_20C6_423B_B274_20A35E4049AA
#define CONFIGDB_H_7FFC7863_20C6_423B_B274_20A35E4049AA

#include "imagecache.h"

#include <QAbstractListModel>
#include <QUrl>

class QFileInfo;

class ConfigDbEntry;

class ConfigDb : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_ENUMS(Roles)

public:

    typedef QHash<int, QByteArray> RoleMap;

    enum Roles
    {
        ConfigRole = Qt::UserRole+1,
    };

    static const int MajorVersion = 1;
    static const int MinorVersion = 0;
    static const QString DatabaseSuffix;

    explicit ConfigDb( QObject *parent = NULL );
    virtual ~ConfigDb();

    QString path() const { return m_path; }

    ImageCache &cache() { return m_cache; }

public slots:

    /// Add path to configuration database
    Q_INVOKABLE void add(const QUrl &url);

    /// Remove configuration from database
    Q_INVOKABLE void remove(int idx);

    /// Return index of selected ConfigDbEntry instance
    Q_INVOKABLE int indexOfConfig(ConfigDbEntry *entry);

    /// Save current content of database to file
    Q_INVOKABLE void save(const QUrl &url = QUrl(""));

    /// Load content of database from file
    Q_INVOKABLE void load(const QUrl &url );

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

signals:

    void pathChanged(QString path);

private slots:

    void onDuplicateConfig();
    void onRemoveConfig();
    void onPurgeConfig();

private:

    void setPath(QString path);

    void addFromPath( const QFileInfo &path );
    void addFromConfig( const QFileInfo &path );
    void addFromRaw( const QFileInfo &path );
    void loadAndAddEntry( ConfigDbEntry *entry );

    void removeAll();

private:

    QString               m_path;
    RoleMap               m_rolemap;
    QList<ConfigDbEntry*> m_configs;
    ImageCache            m_cache;
};

#endif // CONFIGDB_H_7FFC7863_20C6_423B_B274_20A35E4049AA
