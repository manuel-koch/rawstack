#include "configdbentry.h"
#include "configdb.h"
#include "fileinfotoolbox.h"

#include <QDebug>
#include <QFileInfo>

ConfigDbEntry::ConfigDbEntry(QObject *parent)
    : QObject(parent)
    , m_thumbnail(this)
    , m_final(this)
{
    connect( &m_thumbnail, SIGNAL(urlChanged(QUrl)), this, SIGNAL(thumbnailChanged(QUrl)) );
    connect( &m_final,     SIGNAL(urlChanged(QUrl)), this, SIGNAL(finalChanged(QUrl)) );
}

bool ConfigDbEntry::equals(const ConfigDbEntry *other)
{
    if( this == other )
        return true;
    if( !other )
        return false;
    if( m_raw == other->m_raw && m_config == other->m_config )
        return true;
    return false;
}

bool ConfigDbEntry::isValidRaw() const
{
    return FileInfoToolbox::isRaw( QFileInfo(m_raw) );
}

ConfigDb *ConfigDbEntry::db() const
{
     return qobject_cast<ConfigDb*>(parent());
}

void ConfigDbEntry::setRaw(QString raw)
{
    if( m_raw == raw )
        return;

    m_raw = raw;
    qDebug() << "ConfigDbEntry::setRaw()" << m_raw;
    emit rawChanged(m_raw);
}

void ConfigDbEntry::setConfig(QString config)
{
    if( m_config == config )
        return;

    m_config = config;
    qDebug() << "ConfigDbEntry::setConfig()" << m_config;
    emit configChanged(m_config);

    setTitle( QFileInfo(m_config).baseName() );
}

void ConfigDbEntry::setTitle(QString title)
{
    if( m_title == title )
        return;

    m_title = title;
    qDebug() << "ConfigDbEntry::setTitle()" << m_title;
    emit titleChanged(m_title);
}

