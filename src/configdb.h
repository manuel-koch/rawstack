#ifndef CONFIGDB_H
#define CONFIGDB_H

#include <QAbstractListModel>

class QUrl;
class QFileInfo;

class ConfigDbEntry;

class ConfigDb : public QAbstractListModel
{
    Q_OBJECT

public:

    typedef QHash<int, QByteArray> RoleMap;

    enum Roles
    {
        ConfigRole = Qt::UserRole+1,
    };

    explicit ConfigDb( QObject *parent = NULL );
    virtual ~ConfigDb();

public slots:

    /// Add path to configuration database
    Q_INVOKABLE void add(const QUrl &url );

    /// Remove configuration from database
    Q_INVOKABLE void remove(int idx);

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

private:

    void addFromPath( const QFileInfo &path );
    void addFromConfig( const QFileInfo &path );
    void addFromRaw( const QFileInfo &path );
    void addEntry( ConfigDbEntry *entry );

    void removeAll();

private:

    RoleMap               m_rolemap;
    QList<ConfigDbEntry*> m_configs;
};

#endif // CONFIGDB_H
