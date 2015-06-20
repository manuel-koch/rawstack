#ifndef CONFIGBUILDERBASE
#define CONFIGBUILDERBASE

#include <QObject>

class ConfigBuilderBase
{
public:

    explicit ConfigBuilderBase(QString name) : m_name(name) { }
    virtual ~ConfigBuilderBase() { }

    QString name() { return m_name; }

    virtual ConfigBase *create() = 0;

private:

    QString m_name;

};

#endif // CONFIGBUILDERBASE

