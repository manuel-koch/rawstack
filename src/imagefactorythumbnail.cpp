#include "imagefactorythumbnail.h"
#include "configdbentry.h"

#include <exiv2/exiv2.hpp>

#include <QDebug>
#include <QCryptographicHash>
#include <QFileInfo>

ImageFactoryThumbnail::ImageFactoryThumbnail(ConfigDbEntry *config)
    : m_config(config)
{
    connect( m_config, SIGNAL(rawChanged(QString)), this, SLOT(onRawChanged(QString)) );
}

QImage ImageFactoryThumbnail::image()
{
    qDebug() << "ImageFactoryThumbnail::image()" << m_config->raw();
    try
    {
        QFileInfo info(m_config->raw());
        if( !info.exists() )
            return QImage();

        Exiv2::Image::AutoPtr rawImg = Exiv2::ImageFactory::open(m_config->raw().toStdString());
        rawImg->readMetadata();
        qDebug() << "ImageFactoryThumbnail::image() good:" << rawImg->good();
        Exiv2::PreviewManager previewMng( *rawImg );
        Exiv2::PreviewPropertiesList previewProps = previewMng.getPreviewProperties();
        qDebug() << "ImageFactoryThumbnail::image() previews:" << previewProps.size();
        if( previewProps.empty() )
            return QImage();

        // use last/greatest preview image
        Exiv2::PreviewImage previewImg = previewMng.getPreviewImage( previewProps.back() );
        QImage img = QImage::fromData( QByteArray( reinterpret_cast<const char*>(previewImg.pData()), previewImg.size() ) );

        Exiv2::ExifData &exifData = rawImg->exifData();
        Exiv2::ExifKey key("Exif.Image.Orientation");
        Exiv2::ExifMetadata::iterator pos = exifData.findKey(key);
        if( pos != exifData.end() )
        {
            int orientation = pos->value().toLong();
            qDebug() << "ImageFactoryThumbnail::image() orientation:" << pos->value().toString().c_str() << orientation;
            double angle = 0;
            bool   horizMirror = false;
            bool   vertMirror  = false;
            // see http://sylvana.net/jpegcrop/exif_orientation.html
            switch( orientation )
            {
                case 2:
                    horizMirror = true;
                    break;
                case 3:
                    horizMirror = true;
                    vertMirror  = true;
                    break;
                case 4:
                    vertMirror = true;
                    break;
                case 5:
                    angle = 90;
                    vertMirror = true;
                    break;
                case 6:
                    angle = 90;
                    break;
                case 7:
                    angle = -90;
                    horizMirror = true;
                    break;
                case 8:
                    angle = -90;
                    break;
                case 1:
                default:
                    break;
            }

            QMatrix m;
            m.rotate(angle);
            img = img.transformed(m).mirrored(horizMirror,vertMirror);
        }

        return img;
    }
    catch( Exiv2::Error& e )
    {
        qWarning() << "ImageFactoryThumbnail::image() exif failed:" << e.what();
        return QImage();
    }
}

void ImageFactoryThumbnail::onRawChanged(QString raw)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( raw.toLocal8Bit() );
    setUrl( h.result());
}
