#ifndef CONFIGJSONSAVER_H
#define CONFIGJSONSAVER_H

#include "configsaverbase.h"

#include <QDomDocument>

class ConfigXMLSaver : public ConfigSaverBase
{
    Q_OBJECT

public:

    explicit ConfigXMLSaver( QObject *parent = NULL );
    virtual ~ConfigXMLSaver();

    static const int VersionMajor = 1;
    static const int VersionMinor = 0;

    QDomDocument const &doc() { return m_doc; }

    // ConfigSaverBase interface
    virtual void add(ConfigBase *config);

private:

    QDomDocument m_doc;
    QDomNode     m_tasks;
};

#endif // CONFIGJSONSAVER_H
