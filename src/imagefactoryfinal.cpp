#include "imagefactorythumbnail.h"
#include "configdb.h"
#include "configdbentry.h"
#include "imagecache.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QFileInfo>

ImageFactoryFinal::ImageFactoryFinal(ConfigDbEntry *config)
    : m_config(config)
    , m_cycle(0)
{
    connect( m_config, SIGNAL(rawChanged(QString)), this, SLOT(onRawChanged(QString)) );
    connect( &m_config->db()->cache(), SIGNAL(miss(QString)), this, SLOT(onCacheMiss(QString)) );
    connect( &m_config->db()->cache(), SIGNAL(cached(ImageCacheGroup::Action,QString)), this, SLOT(onCacheChanged(ImageCacheGroup::Action,QString)) );
    setUrl( m_cycle );
}

void ImageFactoryFinal::nextUrl()
{
    m_cycle++;
    setUrl( m_cycle );
}

QImage ImageFactoryFinal::image()
{
    // try loading cached full size of exported image
    Magick::Image full = m_config->db()->cache().retrieve(m_config->config(),false);
    if( !full.isValid() )
    {
        // fallback to loading cached full sized embedded in RAW image
        full = m_config->db()->cache().retrieve(m_config->raw(),false);
        if( !full.isValid() )
        {
            qDebug() << "ImageFactoryFinal::image() invalid" << m_config->raw();
            return QImage();
        }
    }

    Magick::Blob blob;
    full.write( &blob );
    qDebug() << "ImageFactoryFinal::image()" << full.size().width() << "x" << full.size().height();
    return QImage::fromData( QByteArray( static_cast<const char*>(blob.data()), blob.length() ) );
}

void ImageFactoryFinal::onRawChanged(QString raw)
{
    Q_UNUSED(raw);
    nextUrl();
}

void ImageFactoryFinal::onConfigChanged(QString config)
{
    Q_UNUSED(config);
    nextUrl();
}

void ImageFactoryFinal::onCacheChanged(ImageCacheGroup::Action action, QString key)
{
    Q_UNUSED(action);
    if( key == m_config->raw() || key == m_config->config() )
        nextUrl();
}

void ImageFactoryFinal::onCacheMiss(QString key)
{
    if( key == m_config->raw() )
        m_config->db()->cache().loadThumbnail(  m_config->raw() );
}