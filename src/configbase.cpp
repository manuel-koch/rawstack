#include "configbase.h"

ConfigBase::ConfigBase(QObject *parent)
    : QObject(parent)
    , m_changes(0)
{
    // EMPTY
}

ConfigBase::~ConfigBase()
{
    // EMPTY
}

void ConfigBase::markDirty()
{
    bool wasClean = m_changes==0;
    m_changes++;
    if( wasClean )
        emit dirtyChanged(true);
}

void ConfigBase::resetDirty()
{
    bool wasDirty = m_changes!=0;
    m_changes = 0;
    if( wasDirty )
        emit dirtyChanged(false);
}
