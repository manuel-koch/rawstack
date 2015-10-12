#include "configxmlsaver.h"
#include "configbase.h"

#include <QDebug>

ConfigXMLSaver::ConfigXMLSaver(QObject *parent)
    : ConfigSaverBase(parent)
{
    m_doc.appendChild( m_doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"") );
    QDomNode root = m_doc.appendChild( m_doc.createElement("rawstack") );
    root.toElement().setAttribute( "version", QString("%1.%2").arg(VersionMajor).arg(VersionMinor) );
    m_tasks    = root.appendChild( m_doc.createElement("tasks") );
}

ConfigXMLSaver::~ConfigXMLSaver()
{
    // EMPTY
}

void ConfigXMLSaver::add(ConfigBase *config)
{
    qDebug() << "ConfigXMLSaver::add()" << config;
    config->toXML( m_tasks );
}
