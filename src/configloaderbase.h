#ifndef CONFIGLOADERBASE_H
#define CONFIGLOADERBASE_H

#include <QObject>

class ConfigBase;

class ConfigLoaderBase : public QObject
{
    Q_OBJECT

public:

    explicit ConfigLoaderBase(QObject *parent = NULL);
    virtual ~ConfigLoaderBase();

signals:

    /// A setting was found in configuration
    void setting(QString key, QString value);

    /// A task config was found in configuration
    void config(ConfigBase *config);
};

#endif // CONFIGLOADERBASE_H
