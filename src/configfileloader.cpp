#include "configfileloader.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

ConfigFileLoader::ConfigFileLoader(QObject *parent)
    : ConfigXmlLoader(parent)
{
    // EMPTY
}

ConfigFileLoader::~ConfigFileLoader()
{
    // EMPTY
}

bool ConfigFileLoader::load(QString path)
{
    qDebug() << "ConfigFileLoader::load()" << path;
    QFile file(path);
    if( file.open( QFile::ReadOnly ) )
    {
        QDomDocument doc;
        QString err;
        int errLine, errCol;
        bool res = doc.setContent(&file,true,&err,&errLine,&errCol);
        if( res )
        {
            return ConfigXmlLoader::load(doc);
        }
        else
            qDebug() << "ConfigFileLoader::load() error at line" << errLine << "col" << errCol << ":" << err;
    }
    else
        qDebug() << "ConfigFileLoader::load() failed to open" << path;

    return false;
}

