#ifndef UFRAWCONFIG_H
#define UFRAWCONFIG_H

#include "configbase.h"

class UfrawConfig : public ConfigBase
{
    Q_OBJECT

public:

    explicit UfrawConfig(QObject *parent);
    virtual ~UfrawConfig();

};

#endif // UFRAWCONFIG_H
