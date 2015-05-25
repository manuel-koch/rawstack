#ifndef UFRAWCONFIG_H
#define UFRAWCONFIG_H

#include "configbase.h"

class UfrawConfig : public ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(double exposure      READ exposure      WRITE setExposure      NOTIFY exposureChanged)
    Q_PROPERTY(int    wbTemperature READ wbTemperature WRITE setWbTemperature NOTIFY wbTemperatureChanged)
    Q_PROPERTY(double wbGreen       READ wbGreen       WRITE setWbGreen       NOTIFY wbGreenChanged)

public:

    explicit UfrawConfig(QObject *parent);
    virtual ~UfrawConfig();

    double exposure() const { return m_exposure; }
    int    wbTemperature() const { return m_wbTemperature; }
    double wbGreen() const { return m_wbGreen; }

public slots:

    void setExposure(double exposure);
    void setWbTemperature(int wbTemperature);
    void setWbGreen(double wbGreen);

signals:

    void exposureChanged(double exposure);
    void wbTemperatureChanged(int wbTemperature);
    void wbGreenChanged(double wbGreen);

private:

    double m_exposure;
    int    m_wbTemperature;
    double m_wbGreen;
};

#endif // UFRAWCONFIG_H
