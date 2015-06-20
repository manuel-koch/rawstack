#ifndef CONFIGBUILDER_H
#define CONFIGBUILDER_H

#include "configbuilderbase.h"

template <typename T>
class ConfigBuilder : public ConfigBuilderBase
{
public:
    explicit ConfigBuilder(QString name) : ConfigBuilderBase(name) { }
    virtual ~ConfigBuilder() { }

    virtual ConfigBase *create()
    {
        return new T();
    }
};

#endif // CONFIGBUILDER_H
