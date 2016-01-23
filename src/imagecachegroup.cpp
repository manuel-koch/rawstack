/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
    , m_memsize(0)
    , m_maxMemSize(128*MByte)
    , m_filesize(0)
    , m_maxFileSize(256*MByte)
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
    if( img.isValid() )
    {
        Magick::Blob blob;
        img.write( &blob );
        emit triggerStore(key,QByteArray( static_cast<const char*>(blob.data()), blob.length() ));
    }
    else
        qWarning() << "ImageCacheGroup::store() skip invalid image" << key;
}

bool ImageCacheGroup::isCached(QString key)
{
    QMutexLocker lock(&m_mutex);
    return m_cached.contains(key);
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
        addEntry( key, QFileInfo( m_dir, hash(key) ).absoluteFilePath() );

    qDebug() << "ImageCacheGroup::storeImpl()" << m_name << key
             << "image:" << img.format().c_str()
             << "width:"  << img.size().width()
             << "height:" << img.size().height();

    m_cached[key]->setImage( img );

    emit cached( Updated, key );

    cleanup();
}

void ImageCacheGroup::onMemSizeChanged(long delta)
{
    m_memsize += delta;
    qDebug() << "ImageCacheGroup::onMemSizeChanged()" << m_name << delta << m_memsize;
}

void ImageCacheGroup::onFileSizeChanged(long delta)
{
    m_filesize += delta;
    qDebug() << "ImageCacheGroup::onFileSizeChanged()" << m_name << delta << m_filesize;
}

void ImageCacheGroup::addEntry(QString key, QString path)
{
    ImageCacheEntry *entry = new ImageCacheEntry(this);

    connect( entry, SIGNAL(memsizeChanged(long)), this, SLOT(onMemSizeChanged(long)) );
    connect( entry, SIGNAL(filesizeChanged(long)), this, SLOT(onFileSizeChanged(long)) );

    entry->setInfo( path );
    m_cached[key] = entry;
    m_used.append(key);
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

    cleanup();

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
    QStringList cachedPaths;
    QFile content( m_dir.absoluteFilePath("cached.xml") );
    if( content.open( QFile::ReadOnly ) )
    {
        QDomDocument doc;
        QString err;
        int errLine, errCol;
        bool res = doc.setContent(&content,true,&err,&errLine,&errCol);
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
            addEntry( key, path.absoluteFilePath() );
            cachedPaths << path.completeBaseName();
        }
    }

    // remove files from directory that are not in loaded list of cached images
    foreach( QString entry, m_dir.entryList(QDir::Files) )
    {
        if( entry == "cached.xml" )
            continue;
        if( !cachedPaths.contains(entry) )
        {
            qDebug() << "ImageCacheGroup::load()" << m_name << "removing obsolete" << entry;
            QFile( m_dir.absoluteFilePath(entry) ).remove();
        }
    }

    qDebug() << "ImageCacheGroup::load()" << m_name << m_cached.size();
}

void ImageCacheGroup::cleanup()
{
    QMutexLocker lock(&m_mutex);

    unsigned long oldMemSize  = m_memsize;
    unsigned long oldFileSize = m_filesize;

    int idx = 0;
    while( m_maxMemSize < m_memsize && idx < m_cached.size() )
    {
        m_cached[ m_used[idx] ]->release(true);
        idx++;
    }

    idx = 0;
    while( m_maxMemSize < m_memsize && idx < m_cached.size() )
    {
        m_cached[ m_used[idx] ]->release(false);
        idx++;
    }

    idx = 0;
    while( m_maxFileSize < m_filesize && idx < m_cached.size() )
    {
        QString key = m_used[idx];
        ImageCacheEntry *entry = m_cached.take(key);
        m_used.removeOne(key);
        entry->purge();
        delete entry;
        idx++;
    }

    if( oldMemSize != m_memsize )
        qDebug() << "ImageCacheGroup::cleanup()" << m_name << "freed mem:" << (oldMemSize-m_memsize);
    if( oldFileSize != m_filesize )
        qDebug() << "ImageCacheGroup::cleanup()" << m_name << "freed file:" << (oldFileSize-m_filesize);
}
