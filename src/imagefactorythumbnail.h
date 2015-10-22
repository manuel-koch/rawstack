#ifndef IMAGEFACTORYTHUMBNAIL_H
#define IMAGEFACTORYTHUMBNAIL_H

#include "imagefactorybase.h"

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

private:

    ConfigDbEntry *m_config;
};

#endif // IMAGEFACTORYTHUMBNAIL_H
