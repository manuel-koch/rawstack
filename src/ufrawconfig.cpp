#include "ufrawconfig.h"
#include "stringtoolbox.h"

#include <QDebug>
#include <QCryptographicHash>

const char *UfrawConfig::Name = "ufraw";

UfrawConfig::UfrawConfig(QObject *parent)
    : ConfigBase(UfrawConfig::Name,parent)
    , m_fuse(UfrawConfig::DefaultFuse)
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
    QDomNode ufrawNode = ConfigBase::toXML(node);

    qDebug() << "UfrawConfig::toXML()";
    QDomText t;
    QDomElement e;

    e = ufrawNode.ownerDocument().createElement("fuse");
    t = ufrawNode.ownerDocument().createTextNode(QString("%1").arg(m_fuse));
    e.appendChild(t);
    ufrawNode.appendChild(e);

    e = ufrawNode.ownerDocument().createElement("exposure");
    t = ufrawNode.ownerDocument().createTextNode(QString("%1").arg(m_exposure));
    e.appendChild(t);
    ufrawNode.appendChild(e);

    e = ufrawNode.ownerDocument().createElement("wbTemperature");
    t = ufrawNode.ownerDocument().createTextNode(QString("%1").arg(m_wbTemperature));
    e.appendChild(t);
    ufrawNode.appendChild(e);

    e = ufrawNode.ownerDocument().createElement("wbGreen");
    t = ufrawNode.ownerDocument().createTextNode(QString("%1").arg(m_wbGreen));
    e.appendChild(t);
    ufrawNode.appendChild(e);

    return ufrawNode;
}

bool UfrawConfig::fromXML(QDomNode const &node )
{
    if( !ConfigBase::fromXML(node) )
        return false;

    qDebug() << "UfrawConfig::fromXML()";

    setFuse( StringToolbox::toInt( node.firstChildElement("fuse").text(), DefaultFuse ) );
    setExposure( StringToolbox::toDouble( node.firstChildElement("exposure").text(), DefaultExposure ) );
    setWbTemperature( StringToolbox::toDouble( node.firstChildElement("wbTemperature").text(), DefaultWbTemperature ) );
    setWbGreen( StringToolbox::toDouble( node.firstChildElement("wbGreen").text(), DefaultWbGreen ) );

    return true;
}

QByteArray UfrawConfig::hash(const QByteArray &baseHash)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( ConfigBase::hash(baseHash) );
    h.addData( m_raw.toLocal8Bit() );
    h.addData( reinterpret_cast<char*>(&m_fuse),          sizeof(m_fuse) );
    h.addData( reinterpret_cast<char*>(&m_exposure),      sizeof(m_exposure) );
    h.addData( reinterpret_cast<char*>(&m_wbTemperature), sizeof(m_wbTemperature) );
    h.addData( reinterpret_cast<char*>(&m_wbGreen),       sizeof(m_wbGreen) );
    return h.result();
}

void UfrawConfig::setExposure(double exposure)
{
    if( m_exposure == exposure )
        return;
    m_exposure = exposure;
    qDebug() << "UfrawConfig::setExposure()" << m_exposure;
    emit exposureChanged(exposure);
    rehash();
}

void UfrawConfig::setWbTemperature(int wbTemperature)
{
    if( m_wbTemperature == wbTemperature )
        return;
    m_wbTemperature = wbTemperature;
    qDebug() << "UfrawConfig::setWbTemperature()" << m_wbTemperature;
    emit wbTemperatureChanged(wbTemperature);
    rehash();
}

void UfrawConfig::setWbGreen(double wbGreen)
{
    if( m_wbGreen == wbGreen )
        return;
    m_wbGreen = wbGreen;
    qDebug() << "UfrawConfig::setWbGreen()" << m_wbGreen;
    emit wbGreenChanged(wbGreen);
    rehash();
}

void UfrawConfig::setRaw(QString raw)
{
    if( m_raw == raw )
        return;
    m_raw = raw;
    qDebug() << "UfrawConfig::setRaw()" << m_raw;
    emit rawChanged(raw);
    rehash();
}

void UfrawConfig::setFuse(int fuse)
{
    if( fuse % 2 == 0 )
        fuse++;
    fuse = std::min( 9, std::max( 1, fuse ) );
    if (m_fuse == fuse)
        return;
    m_fuse = fuse;
    qDebug() << "UfrawConfig::setFuse()" << m_fuse;
    emit fuseChanged(fuse);
    rehash();
}
