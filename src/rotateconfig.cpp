#include "rotateconfig.h"
#include "stringtoolbox.h"

#include <QDebug>
#include <QCryptographicHash>

const char *RotateConfig::Name = "rotate";

RotateConfig::RotateConfig(QObject *parent)
    : ConfigBase(RotateConfig::Name,parent)
    , m_degree(RotateConfig::DefaultDegree)
{
    // EMPTY
}

RotateConfig::~RotateConfig()
{
    // EMPTY
}

QDomNode RotateConfig::toXML(QDomNode &node) const
{
    QDomNode rotateNode = ConfigBase::toXML(node);

    qDebug() << "RotateConfig::toXML()";
    QDomText t;
    QDomElement e;

    e = node.ownerDocument().createElement("degree");
    t = node.ownerDocument().createTextNode(QString("%1").arg(m_degree));
    e.appendChild(t);
    rotateNode.appendChild(e);

    return rotateNode;
}

bool RotateConfig::fromXML(const QDomNode &node)
{
    if( !ConfigBase::fromXML(node) )
        return false;

    qDebug() << "RotateConfig::fromXML()";

    setDegree( StringToolbox::toDouble( node.firstChildElement("degree").text(), DefaultDegree ) );

    return true;
}

QByteArray RotateConfig::hash(const QByteArray &baseHash)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( ConfigBase::hash(baseHash) );
    h.addData( reinterpret_cast<char*>(&m_degree), sizeof(m_degree) );
    return h.result();
}

void RotateConfig::setDegree(double degree)
{
    if( m_degree == degree )
        return;

    m_degree = degree;
    qDebug() << "RotateConfig::setDegree()" << m_degree;
    emit degreeChanged( m_degree );
    rehash();
}
