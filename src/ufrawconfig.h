#ifndef UFRAWCONFIG_H
#define UFRAWCONFIG_H

#include "configbase.h"

class UfrawConfig : public ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(QString raw           READ raw           WRITE setRaw           NOTIFY rawChanged)
    Q_PROPERTY(int     fuse          READ fuse          WRITE setFuse          NOTIFY fuseChanged)
    Q_PROPERTY(double  exposure      READ exposure      WRITE setExposure      NOTIFY exposureChanged)
    Q_PROPERTY(int     wbTemperature READ wbTemperature WRITE setWbTemperature NOTIFY wbTemperatureChanged)
    Q_PROPERTY(double  wbGreen       READ wbGreen       WRITE setWbGreen       NOTIFY wbGreenChanged)

public:

    static const char *Name;
    constexpr static const int    DefaultFuse          = 5;
    constexpr static const double DefaultExposure      = 1.0;
    constexpr static const int    DefaultWbTemperature = 0.0;
    constexpr static const double DefaultWbGreen       = 0.0;

    explicit UfrawConfig(QObject *parent = NULL);
    virtual ~UfrawConfig();

    // ConfigBase interface
    virtual QDomNode toXML(QDomNode &node ) const override;
    virtual bool fromXML( QDomNode const &node ) override;
    virtual QByteArray hash( const QByteArray &baseHash = QByteArray() ) override;

    QString raw() const { return m_raw; }
    int     fuse() const { return m_fuse; }
    double  exposure() const { return m_exposure; }
    int     wbTemperature() const { return m_wbTemperature; }
    double  wbGreen() const { return m_wbGreen; }

public slots:

    void setRaw(QString raw);
    void setFuse(int fuse);
    void setExposure(double exposure);
    void setWbTemperature(int wbTemperature);
    void setWbGreen(double wbGreen);

signals:

    void rawChanged(QString raw);
    void fuseChanged(int fuse);
    void exposureChanged(double exposure);
    void wbTemperatureChanged(int wbTemperature);
    void wbGreenChanged(double wbGreen);

private:

    QString m_raw;
    int     m_fuse;
    double  m_exposure;
    int     m_wbTemperature;
    double  m_wbGreen;
};

#endif // UFRAWCONFIG_H
