#ifndef CONFIGXMLLOADER_H
#define CONFIGXMLLOADER_H

#include "configloaderbase.h"

#include <QObject>
#include <QDomDocument>

class ConfigXmlLoader : public ConfigLoaderBase
{
    Q_OBJECT

public:

    explicit ConfigXmlLoader(QObject *parent = NULL);
    virtual ~ConfigXmlLoader();

    /// load from given document
    bool load(const QDomDocument &doc);
};

#endif // CONFIGXMLLOADER_H
