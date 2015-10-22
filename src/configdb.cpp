#include "configdb.h"
#include "configdbentry.h"
#include "fileinfotoolbox.h"
#include "commonconfig.h"
#include "configfileloader.h"

#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QDir>

ConfigDb::ConfigDb(QObject *parent)
    : QAbstractListModel(parent)
{
    m_rolemap = QAbstractListModel::roleNames();
    m_rolemap[ConfigRole] = "config";
}

ConfigDb::~ConfigDb()
{
    // EMPTY
}

void ConfigDb::add(const QUrl &url)
{
    QFileInfo path(url.toLocalFile());
    if( path.exists() )
        addFromPath( path.absoluteFilePath() );
    else
        qWarning() << "ConfigDb::add() invalid:" << path.absoluteFilePath();
}

void ConfigDb::remove(int idx)
{
    if( idx < 0 || idx >= m_configs.size() )
        return;

    beginRemoveRows( QModelIndex(), idx, idx );
    delete m_configs.takeAt( idx );
    endRemoveRows();
}

void ConfigDb::removeAll()
{
    qDebug() << "ConfigDb::removeAll()";
    beginResetModel();
    while( m_configs.size() )
    {
        delete m_configs.takeLast();
    }
    endResetModel();
}

int ConfigDb::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() )
        return 0;
    qDebug() << "ConfigDb::rowCount()" << m_configs.size();
    return m_configs.size();
}

QVariant ConfigDb::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_configs.size() )
        return QVariant();

    qDebug() << "ConfigDb::data()" << index.row() << m_rolemap[role];
    switch( role )
    {
        case Qt::DisplayRole:
        {
            return m_configs[index.row()]->config();
        }
        case ConfigRole:
        {
            return QVariant::fromValue<QObject*>( m_configs[index.row()] );
        }
    }
    return QVariant();
}

void ConfigDb::addFromPath(const QFileInfo &path)
{
    if( path.isDir() )
    {
        qDebug() << "ConfigDb::addFromPath() adding content of" << path.absoluteFilePath();
        foreach( QFileInfo entry, QDir(path.absoluteFilePath()).entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot) )
        {
            addFromPath( entry.canonicalFilePath() );
        }
        return;
    }

    qDebug() << "ConfigDb::addFromPath()" << path.absoluteFilePath();
    if( path.suffix() == "rawstack" )
        addFromConfig( path );
    else if( FileInfoToolbox::isRaw(path) )
        addFromRaw( path );
}

void ConfigDb::addFromRaw(const QFileInfo &path)
{
    ConfigDbEntry *config = new ConfigDbEntry( this );
    config->setRaw( path.absoluteFilePath() );
    config->setConfig( QFileInfo(path.dir(), path.completeBaseName() + ".rawstack").absoluteFilePath() );
    addEntry( config );
}

void ConfigDb::addFromConfig(const QFileInfo &path)
{
    QString rawPath;
    ConfigFileLoader loader;
    connect( &loader, &ConfigFileLoader::config, [&] (ConfigBase *cfg) {
        if( cfg->name() == "common" )
        {
            CommonConfig *common = reinterpret_cast<CommonConfig*>(cfg);
            rawPath = common->raw();
        }
        delete cfg;
    });
    bool loaded = loader.load(path.absoluteFilePath());
    if( !loaded )
    {
        qDebug() << "ConfigDb::addFromConfig() failed";
        return;
    }

    ConfigDbEntry *config = new ConfigDbEntry( this );
    config->setRaw( rawPath );
    config->setConfig( path.absoluteFilePath() );
    addEntry( config );
}

void ConfigDb::addEntry(ConfigDbEntry *entry)
{
    //FIXME: find out if entry is already in list
    bool found = false;
    foreach( ConfigDbEntry *existingEntry, m_configs )
    {
        if( existingEntry->equals(entry) )
        {
            found = true;
            break;
        }
    }

    if( found )
    {
        qDebug() << "ConfigDb::add() already in list:" << entry->config();
        delete entry;
        return;
    }

    qDebug() << "ConfigDb::add()" << entry->config();

    //FIXME: Insert at correct position ( ordered by path ) instead of appending
    int idx = m_configs.size();
    beginInsertRows( QModelIndex(), idx, idx );
    m_configs.insert( idx, entry );
    endInsertRows();

    qDebug() << "ConfigDb::add()" << m_configs.size();
}
