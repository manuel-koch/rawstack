#include "configbase.h"

ConfigBase::ConfigBase(QObject *parent)
    : QObject(parent)
    , m_dirty(false)
{
    // EMPTY
}

ConfigBase::~ConfigBase()
{
    // EMPTY
}

void ConfigBase::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;
    m_dirty = dirty;
    emit dirtyChanged(m_dirty);
}

