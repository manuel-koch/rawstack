#ifndef CONFIGFILELOADER_H
#define CONFIGFILELOADER_H

#include "configxmlloader.h"

#include <QObject>

class ConfigFileLoader : public ConfigXmlLoader
{
    Q_OBJECT

public:

    explicit ConfigFileLoader(QObject *parent = NULL);
    virtual ~ConfigFileLoader();

    /// load from given path
    bool load(QString path);
};

#endif // CONFIGFILELOADER_H
