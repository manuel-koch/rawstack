#include "thumbnailloader.h"
#include "imagecache.h"

#include <Magick++.h>

#include <exiv2/exiv2.hpp>

#include <QDebug>
#include <QFileInfo>

ThumbnailLoader::ThumbnailLoader(ImageCache &cache, QObject *parent)
    : QObject(parent)
    , m_cache(cache)
{
    connect( this, SIGNAL(load(QString)), this, SLOT(loadImpl(QString)) );
}

void ThumbnailLoader::loadImpl(QString path)
{
    if( path.isEmpty() )
        return;

    if( m_cache.isCached( path ) )
        return;

    qDebug() << "ThumbnailLoader::loadImpl()" << path;
    try
    {
        QFileInfo info(path);
        if( !info.exists() )
            return;

        Exiv2::Image::AutoPtr rawImg = Exiv2::ImageFactory::open(path.toStdString());
        rawImg->readMetadata();
        qDebug() << "ThumbnailLoader::loadImpl() good:" << rawImg->good();
        Exiv2::PreviewManager previewMng( *rawImg );
        Exiv2::PreviewPropertiesList previewProps = previewMng.getPreviewProperties();
        qDebug() << "ThumbnailLoader::loadImpl() previews:" << previewProps.size();
        if( previewProps.empty() )
            return;

        // use last/greatest preview image
        Exiv2::PreviewImage previewImg = previewMng.getPreviewImage( previewProps.back() );
        Magick::Blob blob(previewImg.pData(),previewImg.size());
        Magick::Image img;
        img.read(blob);
        if( !img.isValid() )
            return;

        Exiv2::ExifData &exifData = rawImg->exifData();
        Exiv2::ExifKey key("Exif.Image.Orientation");
        Exiv2::ExifMetadata::iterator pos = exifData.findKey(key);
        if( pos != exifData.end() )
        {
            int orientation = pos->value().toLong();
            qDebug() << "ThumbnailLoader::loadImpl() orientation:" << orientation;
            double degree = 0;
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
                    degree = 90;
                    vertMirror = true;
                    break;
                case 6:
                    degree = 90;
                    break;
                case 7:
                    degree = -90;
                    horizMirror = true;
                    break;
                case 8:
                    degree = -90;
                    break;
                case 1:
                default:
                    break;
            }

            img.rotate(degree);
            if( horizMirror )
                img.flop();
            if( vertMirror )
                img.flip();
        }

        m_cache.store( path, ImageCacheGroup::Persistent, img );
    }
    catch( Exiv2::Error& e )
    {
        qWarning() << "ThumbnailLoader::loadImpl() exif failed:" << e.what();
    }
}

