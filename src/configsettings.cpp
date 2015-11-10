#include "configsettings.h"
#include "configsetting.h"

#include <QDebug>
#include <QList>

ConfigSettings::ConfigSettings(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

ConfigSettings::~ConfigSettings()
{
    // EMPTY
}

QStringList ConfigSettings::tasks() const
{
    QStringList taskList;
    foreach( ConfigSetting *setting, m_settings )
    {
        QString task = setting->task();
        if( !taskList.contains(task) )
            taskList.append(task);
    }
    return taskList;
}

QStringList ConfigSettings::settings(QString prefix) const
{
    if( prefix.isEmpty() )
        return m_map.keys();

    QStringList settings;
    prefix += ".";
    foreach( ConfigSetting *setting, m_settings )
    {
        QString fullname = setting->fullname();
        if( fullname.startsWith(prefix) )
            settings.append(fullname);
    }
    return settings;
}

ConfigSetting *ConfigSettings::getSetting(QString fullname)
{
    if( m_map.contains(fullname) )
        return m_map[fullname];

    ConfigSetting *setting = new ConfigSetting(fullname,this);
    m_settings.append( setting );
    m_map[fullname] = setting;

    return setting;
}

void ConfigSettings::removeSetting(QString name)
{
    QList<int> indices;
    bool wildcard = !name.contains(".");
    for( int i=0; i<m_settings.size(); i++ )
    {
        if( m_settings[i]->fullname() == name || (wildcard && m_settings[i]->task() == name) )
            indices.insert(0,i);
    }
    foreach( int i, indices )
    {
        ConfigSetting *setting = m_settings.takeAt( i );
        m_map.remove( setting->fullname() );
        delete setting;
    }
}

void ConfigSettings::removeAll()
{
    while( m_settings.size() )
    {
        ConfigSetting *setting = m_settings.takeAt(0);
        m_map.remove( setting->fullname() );
        delete setting;
    }
}

void ConfigSettings::toXML(QDomNode &node) const
{
    QDomDocument doc = node.ownerDocument();
    foreach( ConfigSetting *setting, m_settings )
    {
        QDomNode settingNode = node.appendChild( doc.createElement("setting") );
        QDomElement settingElm = settingNode.toElement();
        settingElm.setAttribute("name",setting->fullname());
        QDomText settingText = doc.createTextNode( setting->value().toString() );
        settingElm.appendChild( settingText );
    }
}

void ConfigSettings::fromXML(const QDomNode &node)
{
    removeAll();
    QDomNodeList settings = node.toElement().elementsByTagName("setting");
    for( int i=0; i<settings.size(); i++ )
    {
        QDomElement settingElm = settings.item(i).toElement();
        QString name = settingElm.attribute("name");
        ConfigSetting *setting = getSetting(name);
        setting->setValue( settingElm.text() );
    }
}
