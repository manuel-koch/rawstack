#include "configxmlloader.h"
#include "taskfactory.h"
#include "configbase.h"

#include <QDebug>
#include <QDomNamedNodeMap>

ConfigXmlLoader::ConfigXmlLoader(QObject *parent)
    : ConfigLoaderBase(parent)
{
    // EMPTY
}

ConfigXmlLoader::~ConfigXmlLoader()
{
    // EMPTY
}

bool ConfigXmlLoader::load(const QDomDocument &doc)
{
    qDebug() << "ConfigXmlLoader::load()" << doc.toString();

    QDomElement root     = doc.documentElement();
    QDomElement tasks    = root.firstChildElement("tasks").toElement();

    QDomNodeList children = tasks.childNodes();
    qDebug() << "ConfigXmlLoader::load()" << children.count() << "tasks";
    for( int i=0; i<children.count(); i++)
    {
        QDomElement  elem = children.at(i).toElement();
        QString      name = elem.tagName();
        ConfigBase  *cfg  = TaskFactory::getInstance()->create(name);
        if( cfg && cfg->fromXML(elem) )
        {
            emit config( cfg );
        }
        else
        {
            qDebug() << "ConfigXmlLoader::load() failed for" << name;
            delete cfg;
            return false;
        }
    }
    return true;
}

