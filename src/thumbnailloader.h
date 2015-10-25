#ifndef THUMBNAILLOADER_H
#define THUMBNAILLOADER_H

#include <QObject>

class ImageCache;

class ThumbnailLoader : public QObject
{
    Q_OBJECT

public:

    explicit ThumbnailLoader(ImageCache &cache, QObject *parent = 0);

signals:

    void load(QString path);

public slots:

    void loadImpl(QString path);

private:

    ImageCache &m_cache;
};

#endif // THUMBNAILLOADER_H
