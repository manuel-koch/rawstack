#ifndef CONFIGFILESAVER_H
#define CONFIGFILESAVER_H

#include "configxmlsaver.h"

#include <QObject>
#include <QFile>

class ConfigFileSaver : public ConfigXMLSaver
{
    Q_OBJECT

public:

    explicit ConfigFileSaver( QObject *parent = NULL );
    virtual ~ConfigFileSaver();

    virtual void save(QString path);
};

#endif // CONFIGFILESAVER_H
