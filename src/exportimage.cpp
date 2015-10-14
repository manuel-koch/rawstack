#include "exportimage.h"
#include "taskstack.h"

#include <exiv2/exiv2.hpp>

#include <QDebug>
#include <QDir>
#include <QFileInfo>

ExportImage::ExportImage(QObject *parent)
    : QObject(parent)
    , m_imgQuality(95)
    , m_imgType(JPG)
{
    m_stack = new TaskStack(false,this);
    connect(m_stack, SIGNAL(progressChanged(double)), this, SIGNAL(progressChanged(double)) );
    connect(m_stack, SIGNAL(configChanged(QString)), this, SIGNAL(configChanged(QString)) );
    connect(m_stack, SIGNAL(developingChanged(bool)), this, SLOT(onDeveloped(bool)) );
}

ExportImage::~ExportImage()
{
    // EMPTY
}

QString ExportImage::config() const
{
    return m_stack->config();
}

double ExportImage::progress() const
{
    return m_stack->progress();
}

void ExportImage::exportImage()
{
    m_stack->develop();
}

void ExportImage::setConfig(QString config)
{
    return m_stack->loadFromFile( QUrl::fromLocalFile(config) );
}

void ExportImage::setImgQuality(int imgQuality)
{
    imgQuality = std::min( 100, std::max( 1, imgQuality) );
    if( m_imgQuality == imgQuality )
        return;

    m_imgQuality = imgQuality;
    qDebug() << "ExportImage::setImgQuality()" << m_imgQuality;
    emit imgQualityChanged(m_imgQuality);
}

void ExportImage::setImgType(ExportImage::ImageType imgType)
{
    if( m_imgType == imgType )
        return;

    m_imgType = imgType;
    qDebug() << "ExportImage::setImgType()" << m_imgType;
    emit imgTypeChanged(m_imgType);
}

void ExportImage::setImgPath(QString imgPath)
{
    if( m_imgPath == imgPath )
        return;

    m_imgPath = imgPath;
    qDebug() << "ExportImage::setImgPath()" << m_imgPath;
    emit imgPathChanged(m_imgPath);
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

void ExportImage::onDeveloped(bool developing)
{
    if( developing )
        return;

    qDebug() << "ExportImage::onDeveloped()" << config();

    Magick::Image img = m_stack->gmimage();
    if( !img.isValid() )
        return;

    qDebug() << "ExportImage::onDeveloped()"
             << "result image:" << img.format().c_str()
             << "width:"  << img.size().width()
             << "height:" << img.size().height();

    QFileInfo path( m_imgPath );
    QString ext;
    switch( m_imgType )
    {
        case TIF:
        {
            qDebug() << "ExportImage::onDeveloped() tif";
            ext = ".tif";
            img.magick( "TIF" );
            img.compressType( Magick::ZipCompression );
            img.quality( m_imgQuality );
            break;
        }
        case PNG:
        {
            qDebug() << "ExportImage::onDeveloped() png";
            ext = ".png";
            img.magick( "PNG" );
            img.quality( m_imgQuality );
            break;
        }
        case JPG:
        default:
        {
            qDebug() << "ExportImage::onDeveloped() jpg";
            ext = ".jpg";
            img.magick( "JPG" );
            img.quality( m_imgQuality );
            break;
        }
    }

    setImgPath( QFileInfo( path.dir(), path.completeBaseName() + ext ).absoluteFilePath() );
    qDebug() << "ExportImage::onDeveloped() write" << m_imgPath;
    img.write( m_imgPath.toStdString() );

    qDebug() << "ExportImage::onDeveloped() applying EXIF...";
    QString raw = m_stack->raw();
    try {
        Exiv2::Image::AutoPtr exivRaw = Exiv2::ImageFactory::open(raw.toStdString());
        exivRaw->readMetadata();
        qDebug() << "ExportImage::onDeveloped() raw metadata is" << (exivRaw->good()?"good":"invalid");
        if( exivRaw->good() )
        {
            Exiv2::Image::AutoPtr exivImg = Exiv2::ImageFactory::open(m_imgPath.toStdString());
            exivImg->setMetadata( *exivRaw );

            Exiv2::ExifData &exifData = exivImg->exifData();

            qDebug() << "ExportImage::onDeveloped() fixing orientation...";
            changeExistingExifKey( exifData, "Exif.Image.Orientation", "1" ); /* 1 = Normal orientation */

            qDebug() << "ExportImage::onDeveloped() removing thumbnail data...";

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

            qDebug() << "ExportImage::onDeveloped() removing irrelevant keys...";

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

            qDebug() << "ExportImage::onDeveloped() applying processing software...";

            // Add tool name as processing software
            exifData["Exif.Image.ProcessingSoftware"] = "rawstack";

            qDebug() << "ExportImage::onDeveloped() saving metadata...";

            // Save changes to EXIF data
            exifData.sortByKey();
            exivImg->writeMetadata();
        }
    }
    catch( Exiv2::Error& e )
    {
        qWarning() << "ExportImage::onDeveloped() exif failed:" << e.what();
    }

    qDebug() << "ExportImage::onDeveloped() Done";
}
