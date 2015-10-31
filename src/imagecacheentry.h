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

    /// Returns approx size of image(s) in memory
    int memsize() const { return m_memsize; }

    /// Returns approx size of image(s) in filesystem
    int filesize() const { return m_filesize; }

    /// Reduce memory usage by releasing loaded image(s).
    /// Select whether to keep the thumbnail and only drop the
    /// full size image.
    void release(bool keepThumbnail);

    /// Remove the cached image from filesystem and memory
    void purge();

signals:

    void memsizeChanged(long delta);
    void filesizeChanged(long delta);

private:

    QFileInfo     m_path;       // cached on disk
    Magick::Image m_img;        // full size image cached in memory
    Magick::Image m_thumbnail;  // thumbnail image cached in memory
    int           m_memsize;    // approx size of image(s) in memory
    int           m_filesize;   // approx size of image(s) in filesystem
};

#endif // IMAGECACHEENTRY_H
