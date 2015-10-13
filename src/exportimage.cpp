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
    m_stack = new TaskStack(true,this);
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
        }
        case PNG:
        {
            qDebug() << "ExportImage::onDeveloped() png";
            ext = ".png";
            img.magick( "PNG" );
            img.quality( m_imgQuality );
        }
        case JPG:
        default:
        {
            qDebug() << "ExportImage::onDeveloped() jpg";
            ext = ".jpg";
            img.magick( "JPG" );
            img.quality( m_imgQuality );
        }
    }

    setImgPath( QFileInfo( path.dir(), path.completeBaseName() + ext ).absoluteFilePath() );
    qDebug() << "ExportImage::onDeveloped() write" << m_imgPath;
    img.write( m_imgPath.toStdString() );

    qDebug() << "ExportImage::onDeveloped() copying EXIF...";
    QString raw = m_stack->raw();
    try {
        Exiv2::Image::AutoPtr exivRaw = Exiv2::ImageFactory::open(raw.toStdString());
        exivRaw->readMetadata();
        qDebug() << "ExportImage::onDeveloped() raw metadata is" << (exivRaw->good()?"good":"invalid");
        if( exivRaw->good() )
        {
            Exiv2::Image::AutoPtr exivImg = Exiv2::ImageFactory::open(m_imgPath.toStdString());
            exivImg->setMetadata( *exivRaw );

            Exiv2::ExifData imgExif = exivImg->exifData();

            qDebug() << "ExportImage::onDeveloped() removing orientation...";
            Exiv2::ExifKey orientationKey = Exiv2::ExifKey("Exif.Image.Orientation");
            Exiv2::ExifMetadata::iterator pos = imgExif.findKey(orientationKey);
            if( pos != imgExif.end() )
                imgExif.erase(pos);

            qDebug() << "ExportImage::onDeveloped() removing thumbnail...";
            Exiv2::ExifThumb thumb( imgExif );
            thumb.erase();

            // Remove thumbnail
            Exiv2::ExifData::iterator it;
            if((it = imgExif.findKey(Exiv2::ExifKey("Exif.Thumbnail.Compression"))) != imgExif.end())
               imgExif.erase(it);
            if((it = imgExif.findKey(Exiv2::ExifKey("Exif.Thumbnail.XResolution"))) != imgExif.end())
               imgExif.erase(it);
            if((it = imgExif.findKey(Exiv2::ExifKey("Exif.Thumbnail.YResolution"))) != imgExif.end())
               imgExif.erase(it);
            if((it = imgExif.findKey(Exiv2::ExifKey("Exif.Thumbnail.ResolutionUnit"))) != imgExif.end())
               imgExif.erase(it);
            if((it = imgExif.findKey(Exiv2::ExifKey("Exif.Thumbnail.JPEGInterchangeFormat"))) != imgExif.end())
               imgExif.erase(it);
            if((it = imgExif.findKey(Exiv2::ExifKey("Exif.Thumbnail.JPEGInterchangeFormatLength"))) != imgExif.end())
               imgExif.erase(it);

            exivImg->writeMetadata();
        }
    }
    catch( Exiv2::Error& e )
    {
        qWarning() << "ExportImage::onDeveloped() exif failed:" << e.what();
    }

    qDebug() << "ExportImage::onDeveloped() Done";
}
