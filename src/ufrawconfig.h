#ifndef UFRAWCONFIG_H
#define UFRAWCONFIG_H

#include "configbase.h"

class UfrawConfig : public ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(QString raw READ raw WRITE setRaw NOTIFY rawChanged)
    Q_PROPERTY(double  exposure      READ exposure      WRITE setExposure      NOTIFY exposureChanged)
    Q_PROPERTY(int     wbTemperature READ wbTemperature WRITE setWbTemperature NOTIFY wbTemperatureChanged)
    Q_PROPERTY(double  wbGreen       READ wbGreen       WRITE setWbGreen       NOTIFY wbGreenChanged)

public:

    static const char  *Name;
    constexpr static const double DefaultExposure      = 1.0;
    constexpr static const int DefaultWbTemperature = 0.0;
    constexpr static const double DefaultWbGreen       = 0.0;

    explicit UfrawConfig(QObject *parent);
    virtual ~UfrawConfig();

    // ConfigBase interface
    virtual QJsonObject toJson() const;
    virtual bool fromJson(const QJsonObject &json);

    QString raw() const { return m_raw; }
    double  exposure() const { return m_exposure; }
    int     wbTemperature() const { return m_wbTemperature; }
    double  wbGreen() const { return m_wbGreen; }

public slots:

    void setRaw(QString raw);
    void setExposure(double exposure);
    void setWbTemperature(int wbTemperature);
    void setWbGreen(double wbGreen);

signals:

    void rawChanged(QString raw);
    void exposureChanged(double exposure);
    void wbTemperatureChanged(int wbTemperature);
    void wbGreenChanged(double wbGreen);

private:

    QString m_raw;
    double  m_exposure;
    int     m_wbTemperature;
    double  m_wbGreen;
};

#endif // UFRAWCONFIG_H
