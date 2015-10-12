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

void ConfigSaverBase::add(ConfigBase *config)
{
    Q_UNUSED(config)
}
