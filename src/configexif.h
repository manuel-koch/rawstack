#ifndef CONFIGEXIF_H
#define CONFIGEXIF_H

#include <QObject>
#include <QVariantMap>
#include <QDomNode>

class ConfigExif : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString model       READ model        NOTIFY modelChanged)
    Q_PROPERTY(QString lens        READ lens         NOTIFY lensChanged)
    Q_PROPERTY(QString exposure    READ exposure     NOTIFY exposureChanged)
    Q_PROPERTY(QString fnumber     READ fnumber      NOTIFY fnumberChanged)
    Q_PROPERTY(QString focallength READ focallength  NOTIFY focallengthChanged)
    Q_PROPERTY(QString iso         READ iso          NOTIFY isoChanged)

public:

    explicit ConfigExif(QObject *parent = 0);

    void setModel(QString model);
    void setLens(QString lens);
    void setExposure(QString exposure);
    void setFnumber(QString fnumber);
    void setFocallength(QString focallength);
    void setIso(QString iso);

    void clearTags();

    void load( QString path );

    void toXML(QDomNode &node) const;
    void fromXML(const QDomNode &node);

    QString model() const { return m_model; }
    QString lens() const { return m_lens; }
    QString exposure() const { return m_exposure; }
    QString fnumber() const { return m_fnumber; }
    QString focallength() const { return m_focallength; }
    QString iso() const { return m_iso; }

signals:

    void modelChanged(QString model);
    void lensChanged(QString lens);
    void exposureChanged(QString exposure);
    void fnumberChanged(QString fnumber);
    void focallengthChanged(QString focallength);
    void isoChanged(QString iso);

private:

    QString m_model;
    QString m_lens;
    QString m_exposure;
    QString m_fnumber;
    QString m_focallength;
    QString m_iso;
};

#endif // CONFIGEXIF_H
