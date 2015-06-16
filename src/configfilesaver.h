#ifndef CONFIGFILESAVER_H
#define CONFIGFILESAVER_H

#include "configxmlsaver.h"

#include <QObject>
#include <QFile>

class ConfigFileSaver : public ConfigXMLSaver
{
public:
    ConfigFileSaver( QFile &file, QObject *parent = NULL );
    ~ConfigFileSaver();

    void setFile( QFile file );

    // ConfigSaverBase interface
    virtual void end();

private:

    QFile &m_file;
};

#endif // CONFIGFILESAVER_H
