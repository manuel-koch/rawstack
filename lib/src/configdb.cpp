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
#include "configdb.h"
#include "configdbentry.h"
#include "fileinfotoolbox.h"

#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QDir>

const QString ConfigDb::DatabaseSuffix = "rawstackdb";

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

    qDebug() << "ConfigDb::remove()" << idx << "of" << m_configs.size();

    beginRemoveRows( QModelIndex(), idx, idx );
    ConfigDbEntry *entry = m_configs.takeAt( idx );
    delete entry;
    endRemoveRows();

    if( !m_path.isEmpty() )
        save();
}

int ConfigDb::indexOfConfig(ConfigDbEntry *entry)
{
    return m_configs.indexOf( entry );
}

void ConfigDb::save(const QUrl &url)
{
    QString path;
    if( url.isEmpty() )
    {
        if( !m_path.isEmpty() )
            path = m_path;
        else
        {
            qWarning() << "ConfigDb::save() can't save to empty path";
            return;
        }
    }
    else
        path = url.toLocalFile();

    QFileInfo info( path );
    info = QFileInfo( info.dir(), info.completeBaseName()+"."+DatabaseSuffix);
    if( !info.dir().exists() )
        info.dir().mkpath(".");
    path = info.absoluteFilePath();

    qDebug() << "ConfigDb::save()" << path << "...";

    QDomDocument doc;
    doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"") );
    QDomNode root = doc.appendChild( doc.createElement("database") );
    root.toElement().setAttribute("version",QString("%1.%2").arg(MajorVersion).arg(MinorVersion));
    foreach( ConfigDbEntry *entry, m_configs )
    {
        QDomNode config = root.appendChild( doc.createElement("config") );
        QString path = entry->settings()->hasSettings() ? entry->config() : entry->raw();
        config.toElement().setAttribute( "path", path );
    }

    QFile file( path );
    if( file.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream stream(&file);
        doc.save(stream,QDomNode::EncodingFromDocument);
        qDebug() << "ConfigDb::save()" << path << "done";
        setPath(path);
    }
    else
        qWarning() << "ConfigDb::save() failed" << path;
}

void ConfigDb::load(const QUrl &url)
{
    QString path = url.toLocalFile();
    qDebug() << "ConfigDb::load()" << path;

    QFileInfo info( path );
    if( !info.exists() )
    {
        qWarning() << "ConfigDb::load failed, path does not exist:" << path;
        return;
    }
    if( info.suffix() != DatabaseSuffix )
    {
        qWarning() << "ConfigDb::load failed, wrong extension:" << path;
        return;
    }

    removeAll();

    QFile file(path);
    if( !file.open( QFile::ReadOnly ) )
    {
        qWarning() << "ConfigDb::load failed to open" << path;
        return;
    }

    QDomDocument doc;
    QString err;
    int errLine, errCol;
    bool res = doc.setContent(&file,true,&err,&errLine,&errCol);
    if( !res )
    {
        qWarning() << "ConfigDb::load() error at line" << errLine << "col" << errCol << ":" << err;
        return;
    }

    setPath(path);

    QDomElement database = doc.documentElement();
    QDomNodeList configs = database.elementsByTagName("config");
    qDebug() << configs.size();
    for( int i=0; i<configs.size(); i++ )
    {
        QDomElement cfg = configs.item(i).toElement();
        addFromPath( cfg.attribute("path") );
    }
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
    ConfigDbEntry *curEntry = qobject_cast<ConfigDbEntry*>( sender() );
    if( !curEntry )
        return;

    qDebug() << "ConfigDb::onDuplicateConfig() from" << curEntry->config();

    int instance = curEntry->instance()+1;
    QFileInfo curConfig( curEntry->config() );
    QFileInfo newConfig;
    do
    {
        newConfig = QFileInfo(curConfig.dir(), curConfig.baseName() + QString(".%1.%2").arg(instance).arg(curConfig.suffix()) );
        qDebug() << newConfig.absoluteFilePath();
        instance++;
    }
    while( newConfig.exists() );

    qDebug() << "ConfigDb::onDuplicateConfig() to" << newConfig.absoluteFilePath();

    ConfigDbEntry *newEntry = new ConfigDbEntry( this );
    newEntry->setRaw( curEntry->raw() ); // use the same RAW image, in case loading fails
    newEntry->load( curEntry->config() ); // loading may fail if source config was never been saved yet
    newEntry->setConfig( newConfig.absoluteFilePath() ); // apply the altered config path
    newEntry->save(); // save as new config
    loadAndAddEntry( newEntry );

    qDebug() << "ConfigDb::onDuplicateConfig()" << newEntry->config() << "done";
}

void ConfigDb::onRemoveConfig()
{
    ConfigDbEntry *entry = qobject_cast<ConfigDbEntry*>( sender() );
    if( !entry )
        return;

    qDebug() << "ConfigDb::onRemoveConfig()" << entry->config();

    remove( m_configs.indexOf( entry ) );
}

void ConfigDb::onPurgeConfig()
{
    ConfigDbEntry *entry = qobject_cast<ConfigDbEntry*>( sender() );
    if( !entry )
        return;

    qDebug() << "ConfigDb::onPurgeConfig()" << entry->config();

    QFile config(entry->config());
    if( config.exists() )
        config.remove();

    remove( m_configs.indexOf( entry ) );
}

void ConfigDb::setPath(QString path)
{
    if( m_path == path )
        return;

    m_path = path;
    qDebug() << "ConfigDb::setPath()" << m_path;
    emit pathChanged(m_path);
}

void ConfigDb::addFromPath(const QFileInfo &path)
{
    qDebug() << "ConfigDb::addFromPath()" << path.absoluteFilePath();
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
    qDebug() << "ConfigDb::addFromRaw()" << path.absoluteFilePath();
    ConfigDbEntry *config = new ConfigDbEntry( this );
    config->setRaw( path.absoluteFilePath() );
    config->setConfig( QFileInfo(path.dir(), path.completeBaseName() + ".rawstack").absoluteFilePath() );
    loadAndAddEntry( config );
}

void ConfigDb::addFromConfig(const QFileInfo &path)
{
    qDebug() << "ConfigDb::addFromConfig()" << path.absoluteFilePath();
    ConfigDbEntry *config = new ConfigDbEntry( this );
    config->setConfig( path.absoluteFilePath() );
    loadAndAddEntry( config );
}

void ConfigDb::loadAndAddEntry(ConfigDbEntry *entry)
{
    qDebug() << "ConfigDb::loadAndAddEntry()" << entry->config();
    entry->load();

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
        qDebug() << "ConfigDb::loadAndAddEntry() already in list:" << entry->config();
        delete entry;
        return;
    }

    qDebug() << "ConfigDb::loadAndAddEntry()" << entry->config();

    int idx = 0;
    foreach( ConfigDbEntry *existingEntry, m_configs )
    {
        if( existingEntry->compare( entry ) > 0 )
            break;
        idx++;
    }
    beginInsertRows( QModelIndex(), idx, idx );
    m_configs.insert( idx, entry );
    connect( entry, SIGNAL(duplicate()), this, SLOT(onDuplicateConfig()) );
    connect( entry, SIGNAL(remove()),    this, SLOT(onRemoveConfig()) );
    connect( entry, SIGNAL(purge()),     this, SLOT(onPurgeConfig()) );
    endInsertRows();

    qDebug() << "ConfigDb::loadAndAddEntry()" << m_configs.size();

    if( !m_path.isEmpty() )
        save();
}
