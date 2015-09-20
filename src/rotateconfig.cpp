#include "rotateconfig.h"
#include "stringtoolbox.h"

#include <QDebug>

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
    node = ConfigBase::toXML(node);

    qDebug() << "RotateConfig::toXML()";
    QDomText t;
    QDomElement e;

    e = node.ownerDocument().createElement("degree");
    t = node.ownerDocument().createTextNode(QString("%1").arg(m_degree));
    e.appendChild(t);
    node.appendChild(e);

    return node;
}

bool RotateConfig::fromXML(const QDomNode &node)
{
    if( !ConfigBase::fromXML(node) )
        return false;

    qDebug() << "RotateConfig::fromXML()";

    setDegree( StringToolbox::toDouble( node.firstChildElement("degree").text(), DefaultDegree ) );

    resetDirty();
    return true;
}

void RotateConfig::setDegree(double degree)
{
    if( m_degree == degree )
        return;

    m_degree = degree;
    degreeChanged( m_degree );
}

