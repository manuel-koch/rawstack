/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#include "configexif.h"

#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>

#include <array>
#include <math.h>
#include <exiv2/exiv2.hpp>

static const char *XML_ATTR_VALUE    = "value";
static const char *XML_ELEM_MAKE     = "make";
static const char *XML_ELEM_MODEL    = "model";
static const char *XML_ELEM_LENS     = "lens";
static const char *XML_ELEM_SHUTTER  = "shutter";
static const char *XML_ELEM_APERTURE = "aperture";
static const char *XML_ELEM_FOCALLEN = "focallen";
static const char *XML_ELEM_ISO      = "iso";

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

void ConfigExif::setMake(QString make)
{
    if( m_make == make )
        return;
    m_make = make;
    emit makeChanged(m_make);
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
    setMake("");
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
        setMake( getExifText(pos, exifData) );
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
             << "maker =" << m_make
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

    QDomNode elmNode = node.appendChild( doc.createElement(XML_ELEM_MAKE) );
    QDomElement elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_make ) );

    elmNode = node.appendChild( doc.createElement(XML_ELEM_MODEL) );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_model ) );

    elmNode = node.appendChild( doc.createElement(XML_ELEM_LENS) );
    elm = elmNode.toElement();
    elm.appendChild( doc.createTextNode( m_lens ) );

    elmNode = node.appendChild( doc.createElement(XML_ELEM_SHUTTER) );
    elm = elmNode.toElement();
    elm.setAttribute(XML_ATTR_VALUE,m_shutter);
    elm.appendChild( doc.createTextNode( m_shutterText ) );

    elmNode = node.appendChild( doc.createElement(XML_ELEM_APERTURE) );
    elm = elmNode.toElement();
    elm.setAttribute(XML_ATTR_VALUE,m_aperture);
    elm.appendChild( doc.createTextNode( m_apertureText) );

    elmNode = node.appendChild( doc.createElement(XML_ELEM_FOCALLEN) );
    elm = elmNode.toElement();
    elm.setAttribute(XML_ATTR_VALUE,m_focallen);
    elm.appendChild( doc.createTextNode( m_focallenText ) );

    elmNode = node.appendChild( doc.createElement(XML_ELEM_ISO) );
    elm = elmNode.toElement();
    elm.setAttribute(XML_ATTR_VALUE,m_iso);
    elm.appendChild( doc.createTextNode( m_isoText ) );
}

void ConfigExif::fromXML(const QDomNode &node)
{
    clearTags();

    QDomElement nodeElm = node.toElement();
    QDomElement makerElm = nodeElm.firstChildElement(XML_ELEM_MAKE);
    if( makerElm.isElement() )
        setModel(makerElm.text().trimmed());

    QDomElement modelElm = nodeElm.firstChildElement(XML_ELEM_MODEL);
    if( modelElm.isElement() )
        setModel(modelElm.text().trimmed());

    QDomElement lensElm = nodeElm.firstChildElement(XML_ELEM_LENS);
    if( lensElm.isElement() )
        setLens(lensElm.text().trimmed());

    QDomElement shutterElm = nodeElm.firstChildElement(XML_ELEM_SHUTTER);
    if( shutterElm.isElement() )
    {
        setShutter(shutterElm.attribute(XML_ATTR_VALUE).toFloat());
        setShutterText(shutterElm.text().trimmed());
    }

    QDomElement apertureElm = nodeElm.firstChildElement(XML_ELEM_APERTURE);
    if( apertureElm.isElement() )
    {
        setAperture(apertureElm.attribute(XML_ATTR_VALUE).toFloat());
        setApertureText(apertureElm.text().trimmed());
    }

    QDomElement focallenElm = nodeElm.firstChildElement(XML_ELEM_FOCALLEN);
    if( focallenElm.isElement() )
    {
        setFocallen(focallenElm.attribute(XML_ATTR_VALUE).toFloat());
        setFocallenText(focallenElm.text().trimmed());
    }

    QDomElement isoElm = nodeElm.firstChildElement(XML_ELEM_ISO);
    if( isoElm.isElement() )
    {
        setIso(isoElm.attribute(XML_ATTR_VALUE).toFloat());
        setIsoText(isoElm.text().trimmed());
    }
}
