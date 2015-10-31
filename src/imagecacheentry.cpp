#include "imagecacheentry.h"

#include <QDebug>

unsigned long imgsize( const Magick::Image &img )
{
    if( img.isValid() )
    {
        Magick::Geometry s = img.size();
        return s.width() * s.height() * img.depth() / 8;
    }
    else
        return 0;
}

ImageCacheEntry::ImageCacheEntry(QObject *parent)
    : QObject(parent)
    , m_memsize(0)
    , m_filesize(0)
{
    // EMPTY
}

void ImageCacheEntry::setImage(Magick::Image image)
{
    if( image.isValid() )
    {
        m_img = image;
        m_img.write( m_path.absoluteFilePath().toStdString() );

        unsigned long oldFileSize  = m_filesize;
        m_filesize = m_path.size();
        emit filesizeChanged( m_filesize - oldFileSize );

        m_thumbnail = m_img;
        m_thumbnail.thumbnail( Magick::Geometry("256x256>") );

        unsigned long oldMemSize = m_memsize;
        m_memsize = imgsize(m_img) + imgsize(m_thumbnail);
        emit memsizeChanged( m_memsize - oldMemSize );
    }
    else
    {
        qWarning() << "ImageCacheEntry::setImage() invalid";
    }
}

void ImageCacheEntry::release(bool keepThumbnail)
{
    long delta = 0;

    if( m_img.isValid() )
    {
        delta -= imgsize(m_img);
        m_img = Magick::Image();
    }

    if( !keepThumbnail && m_thumbnail.isValid() )
    {
        delta -= imgsize(m_thumbnail);
        m_thumbnail = Magick::Image();
    }

    if( delta )
    {
        m_memsize += delta;
        emit memsizeChanged( delta );
    }
}

void ImageCacheEntry::purge()
{
    release(false);
    if( m_path.exists() )
        QFile(m_path.absoluteFilePath()).remove();
    m_path = QFileInfo("");
    long delta = -m_filesize;
    m_filesize = 0;
    emit filesizeChanged( delta );
}

Magick::Image ImageCacheEntry::getImage(bool thumbnail)
{
    if( thumbnail && m_thumbnail.isValid() )
        return m_thumbnail;

    if( !thumbnail && m_img.isValid() )
        return m_img;

    long delta = 0;

    qDebug() << "ImageCacheEntry::getImage() reloading" << m_path.absoluteFilePath();
    bool fullsizeLoaded = false;
    QString path = m_path.absoluteFilePath();
    if( !m_img.isValid() && !path.isEmpty() )
    {
        m_img.read( path.toStdString() );
        delta += imgsize(m_img);
        fullsizeLoaded = true;
    }

    bool thumbnailLoaded = false;
    if( m_img.isValid() && !m_thumbnail.isValid() )
    {
        m_thumbnail = m_img;
        m_thumbnail.thumbnail( Magick::Geometry("256x256>") );
        delta += imgsize(m_thumbnail);
        thumbnailLoaded = true;
    }

    if( fullsizeLoaded && thumbnail )
    {
        delta -= imgsize(m_img);
        m_img = Magick::Image();
    }

    m_memsize += delta;
    emit memsizeChanged( delta );

    return thumbnail ? m_thumbnail : m_img;
}
