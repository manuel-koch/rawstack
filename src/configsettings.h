#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QDomNode>

class ConfigSetting;

class ConfigSettings : public QObject
{
    Q_OBJECT

public:

    explicit ConfigSettings( QObject *parent = NULL );
    virtual ~ConfigSettings();

    /// Return list of tasks that have a setting
    QStringList tasks() const;

    /// Return list of all settings or those starting with selected prefix ( task/group )
    QStringList settings(QString prefix = "") const;

    /// Return a named setting
    ConfigSetting *getSetting(QString fullname);

    /// Remove named setting or all settings of named task
    void removeSetting(QString name);

    /// Remove all settings
    void removeAll();

    void toXML(QDomNode &node) const;
    void fromXML(const QDomNode &node);

private:

    QList<ConfigSetting*>         m_settings;
    QHash<QString,ConfigSetting*> m_map;
};

#endif // CONFIGSETTINGS_H
