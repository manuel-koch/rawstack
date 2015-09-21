#include "ufrawconfig.h"
#include "stringtoolbox.h"

#include <QDebug>
#include <QCryptographicHash>

const char *UfrawConfig::Name = "ufraw";

UfrawConfig::UfrawConfig(QObject *parent)
    : ConfigBase(UfrawConfig::Name,parent)
    , m_exposure(UfrawConfig::DefaultExposure)
    , m_wbTemperature(UfrawConfig::DefaultWbTemperature)
    , m_wbGreen(UfrawConfig::DefaultWbGreen)
{
    setCanDisable(false);
}

UfrawConfig::~UfrawConfig()
{
    // EMPTY
}

QDomNode UfrawConfig::toXML( QDomNode &node ) const
{
    node = ConfigBase::toXML(node);

    qDebug() << "UfrawConfig::toXML()";
    QDomText t;
    QDomElement e;

    e = node.ownerDocument().createElement("exposure");
    t = node.ownerDocument().createTextNode(QString("%1").arg(m_exposure));
    e.appendChild(t);
    node.appendChild(e);

    e = node.ownerDocument().createElement("wbTemperature");
    t = node.ownerDocument().createTextNode(QString("%1").arg(m_wbTemperature));
    e.appendChild(t);
    node.appendChild(e);

    e = node.ownerDocument().createElement("wbGreen");
    t = node.ownerDocument().createTextNode(QString("%1").arg(m_wbGreen));
    e.appendChild(t);
    node.appendChild(e);

    return node;
}

bool UfrawConfig::fromXML(QDomNode const &node )
{
    if( !ConfigBase::fromXML(node) )
        return false;

    qDebug() << "UfrawConfig::fromXML()";

    setExposure( StringToolbox::toDouble( node.firstChildElement("exposure").text(), DefaultExposure ) );
    setWbTemperature( StringToolbox::toDouble( node.firstChildElement("wbTemperature").text(), DefaultWbTemperature ) );
    setWbGreen( StringToolbox::toDouble( node.firstChildElement("wbGreen").text(), DefaultWbGreen ) );

    resetDirty();
    return true;
}

QByteArray UfrawConfig::hash(const QByteArray &baseHash)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( ConfigBase::hash(baseHash) );
    h.addData( m_raw.toLocal8Bit() );
    h.addData( reinterpret_cast<char*>(&m_exposure),      sizeof(m_exposure) );
    h.addData( reinterpret_cast<char*>(&m_wbTemperature), sizeof(m_wbTemperature) );
    h.addData( reinterpret_cast<char*>(&m_wbGreen),       sizeof(m_wbGreen) );
    return h.result();
}

void UfrawConfig::setExposure(double exposure)
{
    if (m_exposure == exposure)
        return;
    m_exposure = exposure;
    qDebug() << "UfrawConfig::setExposure()" << m_exposure;
    emit exposureChanged(exposure);
    markDirty();
}

void UfrawConfig::setWbTemperature(int wbTemperature)
{
    if (m_wbTemperature == wbTemperature)
        return;
    m_wbTemperature = wbTemperature;
    qDebug() << "UfrawConfig::setWbTemperature()" << m_wbTemperature;
    emit wbTemperatureChanged(wbTemperature);
    markDirty();
}

void UfrawConfig::setWbGreen(double wbGreen)
{
    if (m_wbGreen == wbGreen)
        return;
    m_wbGreen = wbGreen;
    qDebug() << "UfrawConfig::setWbGreen()" << m_wbGreen;
    emit wbGreenChanged(wbGreen);
    markDirty();
}

void UfrawConfig::setRaw(QString raw)
{
    if (m_raw == raw)
        return;
    m_raw = raw;
    qDebug() << "UfrawConfig::setRaw()" << m_raw;
    emit rawChanged(raw);
    markDirty();
}

