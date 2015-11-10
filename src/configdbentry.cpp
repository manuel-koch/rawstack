#include "configdbentry.h"
#include "configdb.h"
#include "fileinfotoolbox.h"

#include <QDebug>
#include <QFileInfo>
#include <QDomDocument>

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

void ConfigDbEntry::load(QString path)
{
    if( path.isEmpty() )
        path = m_config;

    qDebug() << "ConfigDbEntry::load()" << path;

    m_settings.removeAll();

    QFile file(path);
    if( !file.open( QFile::ReadOnly ) )
    {
        qWarning() << "ConfigDbEntry::load() failed to open" << path;
        return;
    }

    QDomDocument doc;
    QString err;
    int errLine, errCol;
    bool res = doc.setContent(&file,true,&err,&errLine,&errCol);
    if( !res )
    {
        qWarning() << "ConfigDbEntry::load() error at line" << errLine << "col" << errCol << ":" << err;
        return;
    }

    fromXML( doc );

    qDebug() << "ConfigDbEntry::load()" << path << "done";
}

void ConfigDbEntry::fromXML(const QDomDocument &doc)
{
    QDomElement root = doc.documentElement();

    QString   raw     = root.attribute("raw");
    QFileInfo rawInfo = raw;
    if( !rawInfo.exists() )
        rawInfo = QFileInfo( QFileInfo(m_config).dir(), rawInfo.fileName() );

    if( rawInfo.exists() )
        setRaw( rawInfo.absoluteFilePath() );
    else
    {
        qWarning() << "ConfigDbEntry::fromXML() invalid path" << raw;
        setRaw( raw );
    }

    QDomNode settings = root.firstChildElement("settings");
    m_settings.fromXML( settings );
}

void ConfigDbEntry::save(QString path)
{
    if( path.isEmpty() )
        path = m_config;

    qDebug() << "ConfigDbEntry::save()" << path;

    QDomDocument doc;
    doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"") );

    toXML( doc );

    QFile file( path );
    if( file.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream stream(&file);
        doc.save(stream,QDomNode::EncodingFromDocument);
        qDebug() << "ConfigDbEntry::save()" << path << "done";
    }
    else
        qWarning() << "ConfigDbEntry::save() failed" << path;
}

void ConfigDbEntry::toXML(QDomDocument &doc) const
{
    QDomNode    root     = doc.appendChild( doc.createElement("rawstack") );
    QDomElement rootElem = root.toElement();

    rootElem.setAttribute("version","1.0");
    rootElem.setAttribute("raw",m_raw);

    QDomNode settings = root.appendChild( doc.createElement("settings") );
    m_settings.toXML( settings );
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

