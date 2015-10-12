#include "commonconfig.h"

#include <QDebug>
#include <QCryptographicHash>

const char *CommonConfig::Name = "common";

CommonConfig::CommonConfig(QObject *parent)
    : ConfigBase(Name,parent)
{
    setCanDisable(false);
}

CommonConfig::~CommonConfig()
{
    // EMPTY
}

void CommonConfig::setRaw(QString raw)
{
    if (m_raw == raw)
        return;

    m_raw = raw;
    qDebug() << "CommonConfig::setRaw()" << m_raw;
    emit rawChanged(raw);
    rehash();
}

void CommonConfig::setExif(QVariantMap exif)
{
    if (m_exif == exif)
        return;

    m_exif = exif;
    qDebug() << "CommonConfig::setExif()" << m_exif;
    emit exifChanged(exif);
    rehash();
}

QByteArray CommonConfig::hash(const QByteArray &baseHash)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( ConfigBase::hash(baseHash) );
    h.addData( m_raw.toUtf8() );
    for( auto it = m_exif.begin(); it!= m_exif.end(); it++ )
    {
        QString tag = it.key();
        QString val = it.value().toString();
        h.addData( tag.toUtf8() );
        h.addData( val.toUtf8() );
    }
    return h.result();
}

QDomNode CommonConfig::toXML(QDomNode &node) const
{
    QDomNode commonNode = ConfigBase::toXML(node);
    QDomDocument doc = commonNode.ownerDocument();

    qDebug() << "CommonConfig::toXML()";
    QDomText t;
    QDomElement e;

    e = doc.createElement("raw");
    t = doc.createTextNode(QString("%1").arg(m_raw));
    e.appendChild(t);
    commonNode.appendChild(e);

    QDomElement exifNode = doc.createElement("exif");
    commonNode.appendChild(exifNode);

    for( auto it = m_exif.begin(); it!= m_exif.end(); it++ )
    {
        QString tag = it.key();
        QString val = it.value().toString();
        e = doc.createElement(tag.toLower());
        t = doc.createTextNode(val);
        e.appendChild(t);
        exifNode.appendChild(e);
    }

    return commonNode;
}

bool CommonConfig::fromXML(const QDomNode &node)
{
    if( !ConfigBase::fromXML(node) )
        return false;

    qDebug() << "CommonConfig::fromXML()";

    setRaw( node.firstChildElement("raw").text() );

    QVariantMap exif;
    QDomNode exifNode = node.firstChildElement("exif");
    while( exifNode.isElement() )
    {
        exif[exifNode.nodeName()] = exifNode.firstChildElement().text();
        exifNode = exifNode.nextSibling();
    }
    setExif( exif );

    return true;
}
