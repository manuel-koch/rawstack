#include "configexif.h"

#include <QDebug>
#include <QFileInfo>

#include <math.h>
#include <exiv2/exiv2.hpp>

static const Exiv2::ExifKey modelKey("Exif.Image.Model");
static const Exiv2::ExifKey canonLensKey("Exif.Canon.LensModel");
static const Exiv2::ExifKey canonCsLensKey("Exif.CanonCs.Lens");
static const Exiv2::ExifKey exposureKey("Exif.Photo.ExposureTime");
static const Exiv2::ExifKey fnumberKey("Exif.Photo.FNumber");
static const Exiv2::ExifKey focallengthKey("Exif.Photo.FocalLength");
static const Exiv2::ExifKey isoKey("Exif.Photo.ISOSpeedRatings");

ConfigExif::ConfigExif(QObject *parent) : QObject(parent)
{
    // EMPTY
}

void ConfigExif::setModel(QString model)
{
    if( m_model == model )
        return;
    m_model = model;
    emit modelChanged(m_model);
}

void ConfigExif::setLens(QString lens)
{
    if( m_lens == lens )
        return;
    m_lens = lens;
    emit lensChanged(m_lens);
}

void ConfigExif::setExposure(QString exposure)
{
    if( m_exposure == exposure )
        return;
    m_exposure = exposure;
    emit exposureChanged(m_exposure);
}

void ConfigExif::setFnumber(QString fnumber)
{
    if( m_fnumber == fnumber )
        return;
    m_fnumber = fnumber;
    emit fnumberChanged(m_fnumber);
}

void ConfigExif::setFocallength(QString focallength)
{
    if( m_focallength == focallength )
        return;
    m_focallength = focallength;
    emit focallengthChanged(m_focallength);
}

void ConfigExif::setIso(QString iso)
{
    if( m_iso == iso )
        return;
    m_iso = iso;
    emit isoChanged(m_iso);
}

void ConfigExif::clearTags()
{
    setModel("");
    setLens("");
    setExposure("");
    setFnumber("");
    setFocallength("");
    setIso("");
}

void ConfigExif::load(QString path)
{
    qDebug() << "ConfigExif::load()" << path;

    clearTags();

    QFileInfo info(path);
    if( !info.exists() )
        return;

    Exiv2::Image::AutoPtr exifImg = Exiv2::ImageFactory::open(path.toStdString());
    exifImg->readMetadata();
    qDebug() << "ConfigExif::load() good:" << exifImg->good();
    if( !exifImg->good() )
        return;

    Exiv2::ExifData &exifData = exifImg->exifData();
    Exiv2::ExifMetadata::iterator pos;

    QString model;
    pos = exifData.findKey(modelKey);
    if( pos != exifData.end() )
        model = QString::fromStdString( pos->value().toString() );
    qDebug() << "ConfigExif::load() model" << model;
    setModel( model );

    QString lens;
    pos = exifData.findKey(canonLensKey);
    if( pos != exifData.end() )
        lens = QString::fromStdString( pos->value().toString() );
    else {
        pos = exifData.findKey(canonCsLensKey);
        if( pos != exifData.end() )
        {
            short from = pos->value().toLong(0);
            short to   = pos->value().toLong(1);
            lens = QString("%1 - %2 mm").arg(from).arg(to);
        }
    }
    qDebug() << "ConfigExif::load() lens" << lens;
    setLens(lens);

    QString exposure;
    pos = exifData.findKey(exposureKey);
    if( pos != exifData.end() )
        exposure = QString("%1 s").arg(QString::fromStdString( pos->value().toString() ));
    qDebug() << "ConfigExif::load() exposure" << exposure;
    setExposure(exposure);

    QString fnumber;
    pos = exifData.findKey(fnumberKey);
    if( pos != exifData.end() )
    {
        float f = pos->value().toFloat();
        if( f == floor(f) )
            fnumber = QString("F%1").arg(f);
        else
            fnumber = QString("F%1").arg(f,3,'g',2);
    }
    qDebug() << "ConfigExif::load() fnumber" << fnumber;
    setFnumber(fnumber);

    QString focallength;
    pos = exifData.findKey(focallengthKey);
    if( pos != exifData.end() ) {
        float l = pos->value().toFloat();
        if( l == floor(l) )
            focallength = QString("%1 mm").arg(l);
        else
            focallength = QString("%1 mm").arg(l,3,'g',2);
    }
    setFocallength(focallength);
    qDebug() << "ConfigExif::load() focallength" << focallength;

    QString iso;
    pos = exifData.findKey(isoKey);
    if( pos != exifData.end() ) {
        iso = QString("ISO %1").arg(pos->value().toLong());
    }
    qDebug() << "ConfigExif::load() iso" << iso;
    setIso(iso);
}

void ConfigExif::toXML(QDomNode &node) const
{
    QDomDocument doc = node.ownerDocument();

    QDomNode elmNode = node.appendChild( doc.createElement("model") );
    QDomElement elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_model ) );

    elmNode = node.appendChild( doc.createElement("lens") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_lens ) );

    elmNode = node.appendChild( doc.createElement("exposure") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_exposure ) );

    elmNode = node.appendChild( doc.createElement("fnumber") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_fnumber ) );

    elmNode = node.appendChild( doc.createElement("focallength") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_focallength ) );

    elmNode = node.appendChild( doc.createElement("iso") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_iso ) );
}

void ConfigExif::fromXML(const QDomNode &node)
{
    clearTags();

    QDomElement nodeElm = node.toElement();
    QDomElement modelElm = nodeElm.firstChildElement("model");
    if( modelElm.isElement() )
        setModel(modelElm.text().trimmed());

    QDomElement lensElm = nodeElm.firstChildElement("lens");
    if( lensElm.isElement() )
        setLens(lensElm.text().trimmed());

    QDomElement exposureElm = nodeElm.firstChildElement("exposure");
    if( exposureElm.isElement() )
        setExposure(exposureElm.text().trimmed());

    QDomElement fnumberElm = nodeElm.firstChildElement("fnumber");
    if( fnumberElm.isElement() )
        setFnumber(fnumberElm.text().trimmed());

    QDomElement focallengthElm = nodeElm.firstChildElement("focallength");
    if( focallengthElm.isElement() )
        setFocallength(focallengthElm.text().trimmed());

    QDomElement isoElm = nodeElm.firstChildElement("iso");
    if( isoElm.isElement() )
        setIso(isoElm.text().trimmed());
}
