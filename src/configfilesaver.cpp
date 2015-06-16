#include "configfilesaver.h"

#include <QTextStream>

ConfigFileSaver::ConfigFileSaver(QFile &file, QObject *parent)
    : ConfigXMLSaver(parent)
    , m_file(file)
{
    // EMPTY
}

ConfigFileSaver::~ConfigFileSaver()
{
    // EMPTY
}

void ConfigFileSaver::end()
{
    ConfigXMLSaver::end();
    if( m_file.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream stream(&m_file);
        doc().save(stream,QDomNode::EncodingFromDocument);
    }
}

