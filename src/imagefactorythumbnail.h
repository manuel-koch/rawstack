#ifndef IMAGEFACTORYTHUMBNAIL_H
#define IMAGEFACTORYTHUMBNAIL_H

#include "imagefactorybase.h"
#include "imagecachegroup.h"

class ConfigDbEntry;

class ImageFactoryThumbnail : public ImageFactoryBase
{
    Q_OBJECT

public:

    explicit ImageFactoryThumbnail(ConfigDbEntry *config);

public:

    // ImageFactoryBase interface
    virtual QImage image();

private slots:

    void onRawChanged(QString raw);
    void onConfigChanged(QString config);
    void onCacheChanged(ImageCacheGroup::Action action, QString key);
    void onCacheMiss(QString key);

    void nextUrl();

private:

    ConfigDbEntry *m_config;
    int            m_cycle;
};

#endif // IMAGEFACTORYTHUMBNAIL_H
