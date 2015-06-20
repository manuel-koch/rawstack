#include "configxmlloader.h"
#include "taskfactory.h"
#include "configbase.h"

#include <QDebug>
#include <QDomNamedNodeMap>

ConfigXmlLoader::ConfigXmlLoader(QObject *parent)
    : ConfigLoaderBase(parent)
{

}

ConfigXmlLoader::~ConfigXmlLoader()
{

}

bool ConfigXmlLoader::load(const QDomDocument &doc)
{
    qDebug() << "ConfigXmlLoader::load()" << doc.toString();

    QDomElement root     = doc.documentElement();
    QDomElement settings = root.firstChildElement("settings").toElement();
    QDomElement tasks    = root.firstChildElement("tasks").toElement();

    QDomNodeList children = settings.childNodes();
    qDebug() << "ConfigXmlLoader::load()" << children.count() << "settings";
    for( int i=0; i<children.count(); i++)
    {
        QDomElement elem = children.at(i).toElement();
        if( elem.tagName() == "setting" )
        {
            QDomNamedNodeMap attrs = elem.attributes();
            for( int j=0; j<attrs.count(); j++)
            {
                QDomAttr attr = attrs.item(j).toAttr();
                emit setting( attr.name(), attr.value() );
            }
        }
    }

    children = tasks.childNodes();
    qDebug() << "ConfigXmlLoader::load()" << children.count() << "tasks";
    for( int i=0; i<children.count(); i++)
    {
        QDomElement elem = children.at(i).toElement();
        QString      name = elem.tagName();
        ConfigBase  *cfg = TaskFactory::getInstance()->create(name);
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

