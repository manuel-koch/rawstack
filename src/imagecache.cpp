#include "imagecache.h"

#include <QDebug>
#include <QCryptographicHash>

static const size_t MByte = 1024*1024;

ImageCache::ImageCache( QObject *parent )
    : QObject(parent)
    , m_tmpDir("cached_")
    , m_sizeMb(0)
    , m_maxSizeMb(128)
{
    if( !m_tmpDir.isValid() )
        qWarning() << "ImageCache::ImageCache() invalid temp dir";
}

ImageCache::~ImageCache()
{
    // EMPTY
}

Magick::Image ImageCache::retrieve(const std::string &key)
{
    QString k = hash(key);
    Magick::Image img;
    if( m_cached.contains(k) )
    {
        qDebug() << "ImageCache::retrieve() hit" << k;
        QFileInfo path(m_tmpDir.path(),k);
        img.read(path.filePath().toStdString());
    }
    else
        qDebug() << "ImageCache::retrieve() miss" << k;

    return img;
}

void ImageCache::store(const std::string &key, Magick::Image img)
{
    QString k = hash(key);
    QFileInfo path(m_tmpDir.path(),k);

    if( m_cached.contains(k) )
    {
        m_sizeMb -= path.size() / MByte;
        QFile(path.filePath()).remove();
        m_cached.removeOne(k);
    }

    m_cached.append(k);
    img.write( path.filePath().toStdString() );
    m_sizeMb += path.size() / MByte;
    qDebug() << "ImageCache::store()" << k << "size MB" << m_sizeMb;

    reduce();
}

QString ImageCache::hash(const std::string &key)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( key.c_str(), key.size() );
    return  QString::fromLatin1(h.result().toHex());
}

void ImageCache::reduce()
{
    while( !m_cached.empty() && m_sizeMb > m_maxSizeMb )
    {
        QString k = m_cached.takeFirst(); // remove oldest cache entry
        QFileInfo path(m_tmpDir.path(),k);
        m_sizeMb -= path.size() / MByte;
        QFile(path.filePath()).remove();
        qDebug() << "ImageCache::reduce()" << k << "size MB" << m_sizeMb;
    }
}
