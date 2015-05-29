#include "configbase.h"

#include <QDebug>

ConfigBase::ConfigBase(QString name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_enabled(true)
    , m_canDisable(true)
    , m_changes(0)
{
    // EMPTY
}

ConfigBase::~ConfigBase()
{
    // EMPTY
}

QJsonObject ConfigBase::toJson() const
{
    qDebug() << "ConfigBase::toJson()" << this;
    QJsonObject json;
    json["name"]    = m_name;
    json["enabled"] = m_enabled;
    return json;
}

bool ConfigBase::fromJson(const QJsonObject &json)
{
    qDebug() << "ConfigBase::fromJson()" << this;
    if( json["name"].isUndefined() )
        return false;
    m_name = json["name"].toString();
    if( json["enabled"].isUndefined() )
        m_enabled = true;
    else
        m_enabled = json["enabled"].toBool();
    return !m_name.isEmpty();
}

void ConfigBase::setEnabled(bool enabled)
{
    if( (!enabled && !m_canDisable) || m_enabled == enabled )
        return;
    m_enabled = enabled;
    qDebug() << "ConfigBase::setEnabled()" << this << m_enabled;
    emit enabledChanged(m_enabled);
    markDirty();
}

void ConfigBase::setCanDisable(bool canDisable)
{
    if( m_canDisable == canDisable )
        return;
    m_canDisable = canDisable;
    qDebug() << "ConfigBase::setCanDisable()" << this << m_canDisable;
    emit canDisableChanged(m_canDisable);
}

void ConfigBase::markDirty()
{
    bool wasClean = m_changes==0;
    m_changes++;
    if( wasClean )
    {
        qDebug() << "ConfigBase::markDirty()" << this;
        emit dirtyChanged(true);
    }
}

void ConfigBase::resetDirty()
{
    bool wasDirty = m_changes!=0;
    m_changes = 0;
    if( wasDirty )
    {
        qDebug() << "ConfigBase::resetDirty()" << this;
        emit dirtyChanged(false);
    }
}
