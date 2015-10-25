#ifndef IMAGECACHEENTRY_H
#define IMAGECACHEENTRY_H

#include <Magick++.h>

#include <QObject>
#include <QFileInfo>

class ImageCacheEntry : public QObject
{
    Q_OBJECT

public:

    explicit ImageCacheEntry(QObject *parent = 0);

    const QFileInfo &info() const { return m_path; }
    void setInfo( QFileInfo path ) { m_path = path; }

    Magick::Image getImage(bool thumbnail);
    void setImage( Magick::Image image );

    /// Reduce memory usage by releasing loaded image(s).
    /// Select whether to keep the thumbnail and only drop the
    /// full size image.
    void release(bool keepThumbnail);

    /// Remove the cached image from filesystem and memory
    void purge();

private:

    QFileInfo     m_path;       // cached on disk
    Magick::Image m_img;        // full size image cached in memory
    Magick::Image m_thumbnail;  // thumbnail image cached in memory
};

#endif // IMAGECACHEENTRY_H
