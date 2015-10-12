#ifndef COMMONCONFIG_H
#define COMMONCONFIG_H

#include "configbase.h"

#include <QMap>
#include <QVariantMap>

class CommonConfig : public ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(QString     raw  READ raw  WRITE setRaw  NOTIFY rawChanged)
    Q_PROPERTY(QVariantMap exif READ exif WRITE setExif NOTIFY exifChanged)

public:

    static const char *Name;

    explicit CommonConfig(QObject *parent = 0);
    virtual ~CommonConfig();

public slots:

    void setRaw(QString raw);
    void setExif(QVariantMap exif);

signals:

    void rawChanged(QString raw);
    void exifChanged(QVariantMap exif);

public:

    QString raw() const { return m_raw; }
    QVariantMap exif() const { return m_exif; }

    // ConfigBase interface
    virtual QByteArray hash(const QByteArray &baseHash) override;
    virtual QDomNode toXML(QDomNode &node) const override;
    virtual bool fromXML(const QDomNode &node) override;

private:

    QString     m_raw;
    QVariantMap m_exif;
};

#endif // COMMONCONFIG_H
