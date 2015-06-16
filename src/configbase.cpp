#include "configbase.h"
#include "stringtoolbox.h"

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

QDomNode ConfigBase::toXML( QDomNode &node ) const
{
    qDebug() << "ConfigBase::toXML()" << this;
    QDomElement e = node.ownerDocument().createElement(m_name);
    e.setAttribute("enabled",m_enabled);
    return node.appendChild( e );
}

bool ConfigBase::fromXML( QDomNode const &node )
{
    qDebug() << "ConfigBase::fromXML()" << this;
    QDomElement e = node.toElement();
    if( e.tagName() != m_name )
        return false;
    m_enabled = StringToolbox::toBool( e.attribute("enabled"), true );
    return true;
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
