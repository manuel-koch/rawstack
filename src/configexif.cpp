#include "configexif.h"

#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>

#include <array>
#include <math.h>
#include <exiv2/exiv2.hpp>

/*
 * Helper function to copy a exif string from current locale
 * (in which exiv2 often returns strings) to QString.
 */
static QString getExifText( Exiv2::ExifMetadata::const_iterator pos, Exiv2::ExifData& exifData )
{
    return QString::fromStdString( pos->print(&exifData) ).trimmed();
}

ConfigExif::ConfigExif(QObject *parent) : QObject(parent)
{
    // EMPTY
}

void ConfigExif::setMaker(QString maker)
{
    if( m_maker == maker )
        return;
    m_maker = maker;
    emit makerChanged(m_maker);
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

void ConfigExif::setShutter(float shutter)
{
    if( m_shutter == shutter )
        return;
    m_shutter = shutter;
    emit shutterChanged(m_shutter);
}

void ConfigExif::setShutterText(QString shutter)
{
    if( m_shutterText == shutter )
        return;
    m_shutterText = shutter;
    emit shutterTextChanged(m_shutterText);
}

void ConfigExif::setAperture(float aperture)
{
    if( m_aperture == aperture )
        return;
    m_aperture = aperture;
    emit apertureChanged(m_aperture);
}

void ConfigExif::setApertureText(QString aperture)
{
    if( m_apertureText == aperture )
        return;
    m_apertureText = aperture;
    emit apertureTextChanged(m_apertureText);
}

void ConfigExif::setFocallen(float focallen)
{
    if( m_focallen == focallen )
        return;
    m_focallen = focallen;
    emit focallenChanged(m_focallen);
}

void ConfigExif::setFocallenText(QString focallen)
{
    if( m_focallenText == focallen )
        return;
    m_focallenText = focallen;
    emit focallenTextChanged(m_focallenText);
}

void ConfigExif::setIso(int iso)
{
    if( m_iso == iso )
        return;
    m_iso = iso;
    emit isoChanged(m_iso);
}

void ConfigExif::setIsoText(QString iso)
{
    if( m_isoText == iso )
        return;
    m_isoText = iso;
    emit isoTextChanged(m_isoText);
}

void ConfigExif::clearTags()
{
    setMaker("");
    setModel("");
    setLens("");
    setShutter(0);
    setShutterText("");
    setAperture(0);
    setApertureText("");
    setFocallen(0);
    setFocallenText("");
    setIso(0);
    setIsoText("");
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
    Exiv2::ExifMetadata::const_iterator pos;

    /* Read maker name */
    if( (pos = Exiv2::make(exifData)) != exifData.end() )
    {
        setMaker( getExifText(pos, exifData) );
    }
    /* Read model name */
    if ((pos = Exiv2::model(exifData)) != exifData.end())
    {
        setModel( getExifText(pos, exifData) );
    }
    /* Read full lens name */
    if( (pos = Exiv2::lensName(exifData)) != exifData.end() )
    {
        QString lens = getExifText(pos, exifData);
        //for e.g. Sony it can contain "----"
        lens = lens.replace(QRegularExpression("^-*(.*)-*"),"\\1");
        //for Canon it can contain "(65535)" or "(0)" for unknown lenses
        //for Pentax it can contain Unknown (0xHEX)
        if( lens.startsWith("(") || lens.contains("unknown",Qt::CaseInsensitive) )
        {
            lens = ""; // force unknown lens
            const Exiv2::ExifKey canonLensKey("Exif.CanonCs.Lens");
            if( (pos = exifData.findKey(canonLensKey)) != exifData.end() )
            {
                // retry with found components, seem to be focal range ( short/long ) of lens in mm
                if( pos->typeId() == Exiv2::unsignedShort && pos->count() >= 2 )
                {
                    short from = pos->value().toLong(0);
                    short to   = pos->value().toLong(1);
                    if( from > to )
                        std::swap( from, to );
                    lens = QString("%1 - %2 mm").arg(from).arg(to);
                }
            }
        }
        setLens( lens );
    }
    /* Read shutter time */
    if( (pos = Exiv2::exposureTime(exifData)) != exifData.end() )
    {
        setShutter( pos->toFloat() );
        setShutterText( getExifText(pos, exifData) );
    }
    /* Read aperture */
    if( (pos = Exiv2::fNumber(exifData)) != exifData.end() )
    {
        setAperture( pos->toFloat() );
        setApertureText( getExifText(pos, exifData) );
    }
    /* Read ISO speed */
    if( (pos = Exiv2::isoSpeed(exifData)) != exifData.end() )
    {
        setIso( pos->toLong() );
        setIsoText( QString("ISO%1").arg(m_iso) );
    }
    /* Read focal length */
    if( (pos = Exiv2::focalLength(exifData)) != exifData.end() )
    {
        setFocallen( pos->toFloat() );
        setFocallenText( getExifText(pos, exifData) );
    }
    qDebug() << "ConfigExif::load()"
             << "maker =" << m_maker
             << "model =" << m_model
             << "lens =" << m_lens
             << "shutter =" << m_shutter << m_shutterText
             << "aperture =" << m_aperture << m_apertureText
             << "focallen =" << m_focallen << m_focallenText
             << "iso =" << m_iso << m_isoText;
}

void ConfigExif::toXML(QDomNode &node) const
{
    QDomDocument doc = node.ownerDocument();

    QDomNode elmNode = node.appendChild( doc.createElement("maker") );
    QDomElement elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_maker ) );

    elmNode = node.appendChild( doc.createElement("model") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_model ) );

    elmNode = node.appendChild( doc.createElement("lens") );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_lens ) );

    elmNode = node.appendChild( doc.createElement("shutter") );
    elm = elmNode.toElement();
    elm.setAttribute("value",m_shutter);
    elm.appendChild( doc.createTextNode( m_shutterText ) );

    elmNode = node.appendChild( doc.createElement("aperture") );
    elm = elmNode.toElement();
    elm.setAttribute("value",m_aperture);
    elm.appendChild( doc.createTextNode( m_apertureText) );

    elmNode = node.appendChild( doc.createElement("focallen") );
    elm = elmNode.toElement();
    elm.setAttribute("value",m_focallen);
    elm.appendChild( doc.createTextNode( m_focallenText ) );

    elmNode = node.appendChild( doc.createElement("iso") );
    elm = elmNode.toElement();
    elm.setAttribute("value",m_iso);
    elm.appendChild( doc.createTextNode( m_isoText ) );
}

void ConfigExif::fromXML(const QDomNode &node)
{
    clearTags();

    QDomElement nodeElm = node.toElement();
    QDomElement makerElm = nodeElm.firstChildElement("maker");
    if( makerElm.isElement() )
        setModel(makerElm.text().trimmed());

    QDomElement modelElm = nodeElm.firstChildElement("model");
    if( modelElm.isElement() )
        setModel(modelElm.text().trimmed());

    QDomElement lensElm = nodeElm.firstChildElement("lens");
    if( lensElm.isElement() )
        setLens(lensElm.text().trimmed());

    QDomElement shutterElm = nodeElm.firstChildElement("shutter");
    if( shutterElm.isElement() )
    {
        setShutter(shutterElm.attribute("value").toFloat());
        setShutterText(shutterElm.text().trimmed());
    }

    QDomElement apertureElm = nodeElm.firstChildElement("aperture");
    if( apertureElm.isElement() )
    {
        setAperture(apertureElm.attribute("value").toFloat());
        setApertureText(apertureElm.text().trimmed());
    }

    QDomElement focallenElm = nodeElm.firstChildElement("focallen");
    if( focallenElm.isElement() )
    {
        setFocallen(focallenElm.attribute("value").toFloat());
        setFocallenText(focallenElm.text().trimmed());
    }

    QDomElement isoElm = nodeElm.firstChildElement("iso");
    if( isoElm.isElement() )
    {
        setIso(isoElm.attribute("value").toFloat());
        setIsoText(isoElm.text().trimmed());
    }
}
