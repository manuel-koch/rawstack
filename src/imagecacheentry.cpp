#include "imagecacheentry.h"

#include <QDebug>

ImageCacheEntry::ImageCacheEntry(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

void ImageCacheEntry::setImage(Magick::Image image)
{
    if( image.isValid() )
    {
        m_img = image;
        m_img.write( m_path.absoluteFilePath().toStdString() );
        m_thumbnail = m_img;
        m_thumbnail.thumbnail( Magick::Geometry("256x256>") );
    }
    else
    {
        qWarning() << "ImageCacheEntry::setImage() invalid";
    }
}

Magick::Image ImageCacheEntry::getImage(bool thumbnail)
{
    if( thumbnail && m_thumbnail.isValid() )
        return m_thumbnail;

    if( !thumbnail && m_img.isValid() )
        return m_img;

    qDebug() << "ImageCacheEntry::getImage() reloading" << m_path.absoluteFilePath();
    QString path = m_path.absoluteFilePath();
    if( !path.isEmpty() )
        m_img.read( path.toStdString() );

    if( m_img.isValid() && !m_thumbnail.isValid() )
    {
        m_thumbnail = m_img;
        m_thumbnail.thumbnail( Magick::Geometry("256x256>") );
    }

    return thumbnail ? m_thumbnail : m_img;
}

void ImageCacheEntry::purge()
{
    m_img       = Magick::Image();
    m_thumbnail = Magick::Image();
    if( m_path.exists() )
        QFile(m_path.absoluteFilePath()).remove();
    m_path = QFileInfo("");
}
