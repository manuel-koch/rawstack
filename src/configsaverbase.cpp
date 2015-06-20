#include "configsaverbase.h"

ConfigSaverBase::ConfigSaverBase(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

ConfigSaverBase::~ConfigSaverBase()
{
    // EMPTY
}

void ConfigSaverBase::add(QString key, QString value)
{
    Q_UNUSED(key);
    Q_UNUSED(value);
}

void ConfigSaverBase::add(ConfigBase *config)
{
    Q_UNUSED(config)
}
