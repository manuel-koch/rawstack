#include "configfilesaver.h"

#include <QTextStream>

ConfigFileSaver::ConfigFileSaver(QObject *parent)
    : ConfigXMLSaver(parent)
{
    // EMPTY
}

ConfigFileSaver::~ConfigFileSaver()
{
    // EMPTY
}

void ConfigFileSaver::save(QString path)
{
    QFile file(path);
    if( file.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream stream(&file);
        doc().save(stream,QDomNode::EncodingFromDocument);
    }
}

