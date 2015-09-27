#include "configbase.h"
#include "stringtoolbox.h"

#include <QDebug>
#include <QCryptographicHash>

ConfigBase::ConfigBase(QString name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_enabled(true)
    , m_canDisable(true)
{
    // EMPTY
}

ConfigBase::~ConfigBase()
{
    // EMPTY
}

void ConfigBase::rehash()
{
    setHash( hash() );
}

QByteArray ConfigBase::hash()
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( m_name.toLocal8Bit() );
    h.addData( reinterpret_cast<char*>(&m_enabled), sizeof(m_enabled) );
    return h.result();
}

QByteArray ConfigBase::hash(const QByteArray &baseHash)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( baseHash );
    h.addData( m_hash );
    return h.result();
}

void ConfigBase::setHash(const QByteArray &hash)
{
    if( m_hash == hash )
        return;
    m_hash = hash;
    qDebug() << "ConfigBase::setHash()" << this << m_hash.toHex();
    emit hashChanged();
}

QDomNode ConfigBase::toXML( QDomNode &node ) const
{
    qDebug() << "ConfigBase::toXML()" << this << node.nodeName();
    QDomElement e = node.ownerDocument().createElement(m_name);
    if( m_canDisable )
        e.setAttribute("enabled",m_enabled);
    return node.appendChild( e );
}

bool ConfigBase::fromXML( QDomNode const &node )
{
    qDebug() << "ConfigBase::fromXML()" << this;
    QDomElement e = node.toElement();
    if( e.tagName() != m_name )
        return false;
    setEnabled( StringToolbox::toBool( e.attribute("enabled"), true ) );
    return true;
}

void ConfigBase::setEnabled(bool enabled)
{
    if( (!enabled && !m_canDisable) || m_enabled == enabled )
        return;
    m_enabled = enabled;
    qDebug() << "ConfigBase::setEnabled()" << this << m_enabled;
    emit enabledChanged(m_enabled);
    rehash();
}

void ConfigBase::setCanDisable(bool canDisable)
{
    if( m_canDisable == canDisable )
        return;
    m_canDisable = canDisable;
    qDebug() << "ConfigBase::setCanDisable()" << this << m_canDisable;
    emit canDisableChanged(m_canDisable);
}
