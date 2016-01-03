#include "configdbentry.h"
#include "configexif.h"
#include "configdb.h"
#include "fileinfotoolbox.h"

#include <QDebug>
#include <QFileInfo>
#include <QDomDocument>
#include <QDateTime>

ConfigDbEntry::ConfigDbEntry(QObject *parent)
    : QObject(parent)
    , m_thumbnail(this)
    , m_final(this)
{
    connect( &m_thumbnail, &ImageFactoryThumbnail::urlChanged, this, &ConfigDbEntry::thumbnailChanged );
    connect( &m_final,     &ImageFactoryFinal::urlChanged,     this, &ConfigDbEntry::finalChanged     );
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

int ConfigDbEntry::compare(const ConfigDbEntry *other)
{
    QFileInfo i(m_config);
    QFileInfo i_(other->config());

    QString d  = i.dir().absolutePath();
    QString d_ = i_.dir().absolutePath();
    int cmp = d.compare(d_);
    if( cmp != 0 )
        return cmp;

    QString n  = i.baseName();
    QString n_ = i_.baseName();
    cmp = n.compare( n_ );
    if( cmp != 0 )
        return cmp;

    if( m_instance > other->instance() )
        return 1;
    else if( m_instance < other->instance() )
        return -1;
    else
        return 0;
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
        // configuration may not exist yet
        loadExif();
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

    QString   raw        = root.attribute("raw");
    QFileInfo configInfo = QFileInfo(m_config);
    QFileInfo rawInfo    = raw;
    if( !rawInfo.exists() )
        rawInfo = QFileInfo( configInfo.dir(), rawInfo.fileName() );

    if( rawInfo.exists() )
        setRaw( rawInfo.absoluteFilePath() );
    else
    {
        qWarning() << "ConfigDbEntry::fromXML() invalid path" << raw;
        setRaw( raw );
    }

    QDomNode exif = root.firstChildElement("exif");
    m_exif.fromXML( exif );
    if( !exif.isElement() || configInfo.lastModified() < rawInfo.lastModified() )
        loadExif();

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

    QDomNode exif = root.appendChild( doc.createElement("exif") );
    m_exif.toXML( exif );

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

    QFileInfo info(m_config);
    setTitle( info.baseName() );

    QStringList suffixes  = info.completeSuffix().split(".");
    int instance = suffixes.length() >= 2 ? suffixes[suffixes.length()-2].toInt() : 1;
    setInstance( instance );
}

void ConfigDbEntry::loadExif()
{
    qDebug() << "ConfigDbEntry::loadExif()" << m_raw;
    if( !m_raw.isEmpty() )
        m_exif.load( m_raw );
}

void ConfigDbEntry::setTitle(QString title)
{
    if( m_title == title )
        return;

    m_title = title;
    qDebug() << "ConfigDbEntry::setTitle()" << m_title;
    emit titleChanged(m_title);
}

void ConfigDbEntry::setInstance(int instance)
{
    if( m_instance == instance )
        return;

    m_instance = instance;
    qDebug() << "ConfigDbEntry::setInstance()" << m_instance;
    emit instanceChanged(m_instance);
}

