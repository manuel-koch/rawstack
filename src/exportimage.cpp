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
#include "exportimage.h"
#include "exportsetting.h"
#include "configdbentry.h"
#include "taskstack.h"

#include <exiv2/exiv2.hpp>

#include <QDebug>
#include <QDir>
#include <QFileInfo>

ExportImage::ExportImage(QThread *workerThread, ExportSetting *setting, QObject *parent)
    : QObject(parent)
    , m_stack(NULL)
    , m_setting(NULL)
    , m_exporting(false)
{
    m_setting = setting;
    m_setting->setParent(this);
    qDebug() << "ExportImage::ExportImage()" << m_setting->config();

    m_stack = new TaskStack(false,this);
    connect(m_stack, SIGNAL(progressChanged(double)), this, SIGNAL(progressChanged(double)) );
    connect(m_stack, SIGNAL(developingChanged(bool)), this, SLOT(onDeveloping(bool)) );
    m_stack->setWorkerThread( workerThread );
    m_stack->setConfig( m_setting->config() );
}

ExportImage::~ExportImage()
{
    // EMPTY
}

double ExportImage::progress() const
{
    return m_stack->progress();
}

void ExportImage::exportImage()
{
    m_stack->develop();
}

void ExportImage::setExporting(bool exporting)
{
    if( m_exporting == exporting )
        return;
    m_exporting = exporting;
    qDebug() << "ExportImage::setExporting()" << m_exporting;
    emit exportingChanged(m_exporting);
}

void removeExifKey( Exiv2::ExifData &exifData, const std::string &name )
{
    Exiv2::ExifKey key(name);
    Exiv2::ExifMetadata::iterator pos = exifData.findKey(key);
    if( pos != exifData.end() )
       exifData.erase(pos);
}

template <typename T>
void changeExistingExifKey( Exiv2::ExifData &exifData, const std::string &name, const T &val )
{
    Exiv2::ExifKey key(name);
    Exiv2::ExifMetadata::iterator pos = exifData.findKey(key);
    if( pos != exifData.end() )
       pos->setValue( val );
}

void ExportImage::onDeveloping(bool developing)
{
    if( developing )
    {
        setExporting(true);
        return;
    }

    qDebug() << "ExportImage::onDeveloped()" << m_setting->config();

    if( saveImage() )
        applyExif();

    m_stack->releaseImages();
    setExporting(false);
    emit exported();

    qDebug() << "ExportImage::onDeveloped() exported";
}

bool ExportImage::saveImage()
{
    if( m_setting->imgPath().isEmpty() )
    {
        qWarning() << "ExportImage::saveImage() empty output path";
        return false;
    }

    Magick::Image img = m_stack->gmimage();
    if( !img.isValid() )
        return false;

    qDebug() << "ExportImage::saveImage()"
             << "result image:" << img.format().c_str()
             << "width:"  << img.size().width()
             << "height:" << img.size().height();

    switch( m_setting->imgType() )
    {
        case ExportSetting::TIF:
        {
            qDebug() << "ExportImage::saveImage() tif";
            img.magick( "TIF" );
            img.compressType( Magick::ZipCompression );
            img.quality( m_setting->imgQuality() );
            break;
        }
        case ExportSetting::PNG:
        {
            qDebug() << "ExportImage::saveImage() png";
            img.magick( "PNG" );
            img.quality( m_setting->imgQuality() );
            break;
        }
        case ExportSetting::JPG:
        default:
        {
            qDebug() << "ExportImage::saveImage() jpg";
            img.magick( "JPG" );
            img.quality( m_setting->imgQuality() );
            break;
        }
    }

    QFileInfo output(m_setting->imgPath());
    QDir      outDir(output.dir());
    if( !outDir.exists() )
    {
        if( !outDir.mkpath(".") )
        {
            qWarning() << "ExportImage::saveImage() failed to create output directory" << outDir.canonicalPath();
            return false;
        }
    }

    qDebug() << "ExportImage::saveImage() write" << output.absoluteFilePath();
    try
    {
        img.write( m_setting->imgPath().toStdString() );
    }
    catch( Magick::ErrorFileOpen &e )
    {
        qWarning() << "ExportImage::saveImage() write failed:" << e.what();
        return false;
    }

    return true;
}

void ExportImage::applyExif()
{
    qDebug() << "ExportImage::applyExif() applying EXIF...";
    QString raw = m_setting->config()->raw();
    try {
        Exiv2::Image::AutoPtr exivRaw = Exiv2::ImageFactory::open(raw.toStdString());
        exivRaw->readMetadata();
        qDebug() << "ExportImage::applyExif() raw metadata is" << (exivRaw->good()?"good":"invalid");
        if( exivRaw->good() )
        {
            Exiv2::Image::AutoPtr exivImg = Exiv2::ImageFactory::open(m_setting->imgPath().toStdString());
            exivImg->setMetadata( *exivRaw );

            Exiv2::ExifData &exifData = exivImg->exifData();

            qDebug() << "ExportImage::applyExif() fixing orientation...";
            changeExistingExifKey( exifData, "Exif.Image.Orientation", "1" ); /* 1 = Normal orientation */

            qDebug() << "ExportImage::applyExif() removing thumbnail data...";

            // Remove thumbnail keys
            Exiv2::ExifThumb thumb( exifData );
            thumb.erase();
            removeExifKey(exifData,"Exif.Thumbnail.Compression");
            removeExifKey(exifData,"Exif.Thumbnail.XResolution");
            removeExifKey(exifData,"Exif.Thumbnail.YResolution");
            removeExifKey(exifData,"Exif.Thumbnail.ResolutionUnit");
            removeExifKey(exifData,"Exif.Thumbnail.JPEGInterchangeFormat");
            removeExifKey(exifData,"Exif.Thumbnail.JPEGInterchangeFormatLength");
            removeExifKey(exifData,"Exif.Nikon3.Preview");
            removeExifKey(exifData,"Exif.NikonPreview.JPEGInterchangeFormat");
            removeExifKey(exifData,"Exif.Pentax.PreviewResolution");
            removeExifKey(exifData,"Exif.Pentax.PreviewLength");
            removeExifKey(exifData,"Exif.Pentax.PreviewOffset");
            removeExifKey(exifData,"Exif.Minolta.Thumbnail");
            removeExifKey(exifData,"Exif.Minolta.ThumbnailOffset");
            removeExifKey(exifData,"Exif.Minolta.ThumbnailLength");
            removeExifKey(exifData,"Exif.Olympus.Thumbnail");
            removeExifKey(exifData,"Exif.Olympus.ThumbnailOffset");
            removeExifKey(exifData,"Exif.Olympus.ThumbnailLength");

            qDebug() << "ExportImage::applyExif() removing irrelevant keys...";

            // Delete original TIFF data, which is irrelevant
            removeExifKey(exifData,"Exif.Image.ImageWidth");
            removeExifKey(exifData,"Exif.Image.ImageLength");
            removeExifKey(exifData,"Exif.Image.BitsPerSample");
            removeExifKey(exifData,"Exif.Image.Compression");
            removeExifKey(exifData,"Exif.Image.PhotometricInterpretation");
            removeExifKey(exifData,"Exif.Image.FillOrder");
            removeExifKey(exifData,"Exif.Image.SamplesPerPixel");
            removeExifKey(exifData,"Exif.Image.StripOffsets");
            removeExifKey(exifData,"Exif.Image.RowsPerStrip");
            removeExifKey(exifData,"Exif.Image.StripByteCounts");
            removeExifKey(exifData,"Exif.Image.XResolution");
            removeExifKey(exifData,"Exif.Image.YResolution");
            removeExifKey(exifData,"Exif.Image.PlanarConfiguration");
            removeExifKey(exifData,"Exif.Image.ResolutionUnit");
            removeExifKey(exifData,"Exif.Photo.PixelXDimension");
            removeExifKey(exifData,"Exif.Photo.PixelYDimension");

            // Delete various MakerNote fields only applicable to the raw file
            removeExifKey(exifData,"Exif.Image.DNGVersion");
            removeExifKey(exifData,"Exif.Image.DNGPrivateData");

            qDebug() << "ExportImage::applyExif() applying processing software...";

            // Add tool name as processing software
            exifData["Exif.Image.ProcessingSoftware"] = "rawstack";

            qDebug() << "ExportImage::applyExif() saving metadata...";

            // Save changes to EXIF data
            exifData.sortByKey();
            exivImg->writeMetadata();
        }
    }
    catch( Exiv2::Error& e )
    {
        qWarning() << "ExportImage::applyExif() exif failed:" << e.what();
    }
}
