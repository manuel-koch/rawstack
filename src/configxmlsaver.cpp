#include "configxmlsaver.h"
#include "configbase.h"

#include <QDebug>
#include <QJsonArray>

ConfigXMLSaver::ConfigXMLSaver(QObject *parent)
    : ConfigSaverBase(parent)
{
    // EMPTY
}

ConfigXMLSaver::~ConfigXMLSaver()
{
    // EMPTY
}

void ConfigXMLSaver::begin()
{
    qDebug() << "ConfigXMLSaver::begin()";
    m_doc.clear();
    m_doc.appendChild( m_doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"") );

    QDomNode root = m_doc.appendChild( m_doc.createElement("rawstack") );
    root.toElement().setAttribute( "version", QString("%1.%2").arg(VersionMajor).arg(VersionMinor) );
    m_settings = root.appendChild( m_doc.createElement("settings") );
    m_tasks    = root.appendChild( m_doc.createElement("tasks") );
}

void ConfigXMLSaver::add(QString key, QString value)
{
    qDebug() << "ConfigXMLSaver::add()" << key << value;
    QDomElement e = m_doc.createElement("setting");;
    e.setAttribute(key,value);
    m_settings.appendChild(e);
}

void ConfigXMLSaver::add(ConfigBase *config)
{
    qDebug() << "ConfigXMLSaver::add()" << config;
    config->toXML( m_tasks );
}

void ConfigXMLSaver::end()
{
    qDebug() << "ConfigXMLSaver::end()" << m_doc.toString();
}

