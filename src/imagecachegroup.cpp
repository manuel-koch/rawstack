#include "imagecachegroup.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QMutexLocker>
#include <QDomDocument>
#include <QDomNode>

static const size_t MByte = 1024*1024;

QString hash(QString key)
{
    QCryptographicHash h(QCryptographicHash::Md5);
    h.addData( key.toUtf8() );
    return  QString::fromLatin1(h.result().toHex());
}

ImageCacheGroup::ImageCacheGroup(QString name, Lifetime lifetime, QObject *parent)
    : QObject(parent)
    , m_mutex(QMutex::Recursive)
    , m_name(name)
    , m_lifetime(lifetime)
{
    m_dir = QFileInfo(QStandardPaths::writableLocation(QStandardPaths::CacheLocation),m_name).absoluteFilePath();
    if( !m_dir.exists() )
        qDebug() << m_dir.mkpath(".");
    qDebug() << "ImageCacheGroup::ImageCacheGroup()" << m_dir.absolutePath();

    connect( this, SIGNAL(triggerStore(QString,QByteArray)),
             this, SLOT(storeImpl(QString,QByteArray)) );
}

ImageCacheGroup::~ImageCacheGroup()
{
    QMutexLocker lock(&m_mutex);

    qDebug() << "ImageCacheGroup::~ImageCacheGroup()" << m_name;

    m_cached.clear();
    m_used.clear();
    if( m_lifetime == Temporary )
        m_dir.removeRecursively();
}

void ImageCacheGroup::store(QString key, Magick::Image img)
{
    Magick::Blob blob;
    img.write( &blob );
    emit triggerStore(key,QByteArray( static_cast<const char*>(blob.data()), blob.length() ));
}

void ImageCacheGroup::storeImpl(QString key, QByteArray blob)
{
    QMutexLocker lock(&m_mutex);

    Magick::Blob blob_(blob.constData(),blob.size());
    Magick::Image img( blob_ );

    if( !img.isValid() )
    {
        qWarning() << "ImageCacheGroup::storeImpl() invalid";
        return;
    }

    if( !m_cached.contains(key) )
    {
        ImageCacheEntry *entry = new ImageCacheEntry(this);
        entry->setInfo( QFileInfo( m_dir, hash(key) ).absoluteFilePath() );
        m_cached[key] = entry;
        m_used.append(key);
    }

    qDebug() << "ImageCacheGroup::storeImpl()" << m_name << key
             << "image:" << img.format().c_str()
             << "width:"  << img.size().width()
             << "height:" << img.size().height();
    m_cached[key]->setImage( img );
    emit cached( Updated, key );
}

Magick::Image ImageCacheGroup::retrieve(QString key, bool thumbnail)
{
    QMutexLocker lock(&m_mutex);

    Magick::Image img;

    if( m_cached.contains(key) )
    {
        m_used.removeOne(key);
        m_used.append(key);
        img = m_cached[key]->getImage(thumbnail);
        qDebug() << "ImageCacheGroup::retrieve()" << m_name << "hit" << (img.isValid()?"valid":"corrupt") << key;
    }
    else
    {
        qDebug() << "ImageCacheGroup::retrieve()" << m_name << "miss" << key;
    }

    return img;
}

void ImageCacheGroup::save()
{
    QMutexLocker lock(&m_mutex);
    qDebug() << "ImageCacheGroup::save()" << m_name << m_cached.size();
    QDomDocument doc;
    doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"") );
    QDomNode root = doc.appendChild( doc.createElement("cached") );
    foreach( QString key, m_used )
    {
        QDomNode entry = root.appendChild( doc.createElement("image") );
        QDomElement elem = entry.toElement();
        elem.setAttribute("key",key);
        elem.setAttribute("img",m_cached[key]->info().fileName());
    }

    QFile file( m_dir.absoluteFilePath("cached.xml") );
    if( file.open( QFile::WriteOnly | QFile::Truncate ) )
    {
        QTextStream stream(&file);
        doc.save(stream,QDomNode::EncodingFromDocument);
        qDebug() << "ImageCacheGroup::save()" << m_name << "done";
    }
}

void ImageCacheGroup::load()
{
    QMutexLocker lock(&m_mutex);
    qDebug() << "ImageCacheGroup::load()" << m_name;
    QFile file( m_dir.absoluteFilePath("cached.xml") );
    if( file.open( QFile::ReadOnly ) )
    {
        QDomDocument doc;
        QString err;
        int errLine, errCol;
        bool res = doc.setContent(&file,true,&err,&errLine,&errCol);
        if( !res )
        {
            qDebug() << "ImageCacheGroup::load() error at line" << errLine << "col" << errCol << ":" << err;
            return;
        }
        QDomElement root  = doc.documentElement();
        QDomNodeList children = root.childNodes();
        for( int i=0; i<children.count(); i++ )
        {
            QDomElement elem = children.at(i).toElement();
            if( !elem.isElement() )
                continue;
            QString key = elem.attribute("key");
            QString img = elem.attribute("img");
            QFileInfo path( m_dir, img );
            if( m_cached.contains(key) || !path.exists() )
                continue;
            ImageCacheEntry *entry = new ImageCacheEntry(this);
            entry->setInfo( path );
            m_cached[key] = entry;
            m_used.append(key);
        }
    }
    qDebug() << "ImageCacheGroup::load()" << m_name << m_cached.size();
}

void ImageCacheGroup::cleanup()
{
    QMutexLocker lock(&m_mutex);
    //TODO
}

