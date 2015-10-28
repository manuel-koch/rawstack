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
    ConfigDbEntry *entry = m_configs.takeAt( idx );
    disconnect( entry, SIGNAL(duplicate()), this, SLOT(onDuplicateConfig()) );
    delete entry;
    endRemoveRows();
}

int ConfigDb::indexOfConfig(ConfigDbEntry *entry)
{
    return m_configs.indexOf( entry );
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

void ConfigDb::onDuplicateConfig()
{
    ConfigDbEntry *entry = qobject_cast<ConfigDbEntry*>( sender() );
    if( !entry )
        return;

    qDebug() << "ConfigDb::onDuplicateConfig() from" << entry->config();

    int idx = 2;
    QFileInfo curConfig( entry->config() );
    QFileInfo newConfig;
    do
    {
        newConfig = QFileInfo(curConfig.dir(), curConfig.baseName() + QString(".%1.%2").arg(idx).arg(curConfig.suffix()) );
        qDebug() << newConfig.absoluteFilePath();
        idx++;
    }
    while( newConfig.exists() );

    qDebug() << "ConfigDb::onDuplicateConfig() to" << newConfig.absoluteFilePath();

    QFile curCfg( entry->config() );
    curCfg.copy( newConfig.absoluteFilePath() );

    ConfigDbEntry *config = new ConfigDbEntry( this );
    config->setRaw( entry->raw() );
    config->setConfig( newConfig.absoluteFilePath() );
    addEntry( config );
}

void ConfigDb::onRemoveConfig()
{
    ConfigDbEntry *entry = qobject_cast<ConfigDbEntry*>( sender() );
    if( !entry )
        return;

    remove( m_configs.indexOf( entry ) );
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

    int idx = 0;
    foreach( ConfigDbEntry *existingEntry, m_configs )
    {
        if( existingEntry->config().compare( entry->config() ) > 0 )
            break;
        idx++;
    }
    beginInsertRows( QModelIndex(), idx, idx );
    m_configs.insert( idx, entry );
    connect( entry, SIGNAL(duplicate()), this, SLOT(onDuplicateConfig()) );
    connect( entry, SIGNAL(remove()),    this, SLOT(onRemoveConfig()) );
    endInsertRows();

    qDebug() << "ConfigDb::add()" << m_configs.size();
}