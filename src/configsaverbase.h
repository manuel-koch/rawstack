#ifndef CONFIGSAVERBASE_H
#define CONFIGSAVERBASE_H

#include <QObject>
#include <QVariant>

class ConfigBase;

class ConfigSaverBase : public QObject
{
    Q_OBJECT

public:

    explicit ConfigSaverBase(QObject *parent = NULL);
    ~ConfigSaverBase();

    virtual void begin();
    virtual void add(QString key, QString value);
    virtual void add(ConfigBase *config);
    virtual void end();
};

#endif // CONFIGSAVERBASE_H
