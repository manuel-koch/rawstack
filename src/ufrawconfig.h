#ifndef UFRAWCONFIG_H
#define UFRAWCONFIG_H

#include "configbase.h"

class UfrawConfig : public ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(double exposure READ exposure WRITE setExposure NOTIFY exposureChanged)

public:

    explicit UfrawConfig(QObject *parent);
    virtual ~UfrawConfig();

    double exposure() const { return m_exposure; }

public slots:

    void setExposure(double exposure);

signals:

    void exposureChanged(double exposure);

private:

    double m_exposure;
};

#endif // UFRAWCONFIG_H
