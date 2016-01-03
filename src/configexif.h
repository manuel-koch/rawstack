#ifndef CONFIGEXIF_H
#define CONFIGEXIF_H

#include <QObject>
#include <QVariantMap>
#include <QDomNode>

class ConfigExif : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString make         READ make         NOTIFY makeChanged)
    Q_PROPERTY(QString model        READ model        NOTIFY modelChanged)
    Q_PROPERTY(QString lens         READ lens         NOTIFY lensChanged)
    Q_PROPERTY(float   shutter      READ shutter      NOTIFY shutterChanged)
    Q_PROPERTY(QString shutterText  READ shutterText  NOTIFY shutterTextChanged)
    Q_PROPERTY(float   aperture     READ aperture     NOTIFY apertureChanged)
    Q_PROPERTY(QString apertureText READ apertureText NOTIFY apertureTextChanged)
    Q_PROPERTY(float   focallen     READ focallen     NOTIFY focallenChanged)
    Q_PROPERTY(QString focallenText READ focallenText NOTIFY focallenTextChanged)
    Q_PROPERTY(int     iso          READ iso          NOTIFY isoChanged)
    Q_PROPERTY(QString isoText      READ isoText      NOTIFY isoTextChanged)

public:

    explicit ConfigExif(QObject *parent = 0);

    void setMake(QString make);
    void setModel(QString model);
    void setLens(QString lens);
    void setShutter(float shutter);
    void setShutterText(QString shutter);
    void setAperture(float aperture);
    void setApertureText(QString aperture);
    void setFocallen(float focallength);
    void setFocallenText(QString focallength);
    void setIso(int iso);
    void setIsoText(QString iso);

    void clearTags();

    void load( QString path );

    void toXML(QDomNode &node) const;
    void fromXML(const QDomNode &node);

    QString make() const { return m_make; }
    QString model() const { return m_model; }
    QString lens() const { return m_lens; }
    float shutter() const { return m_shutter; }
    QString shutterText() const { return m_shutterText; }
    float aperture() const { return m_aperture; }
    QString apertureText() const { return m_apertureText; }
    float focallen() const { return m_focallen; }
    QString focallenText() const { return m_focallenText; }
    int iso() const { return m_iso; }
    QString isoText() const { return m_isoText; }

signals:

    void makeChanged(QString make);
    void modelChanged(QString model);
    void lensChanged(QString lens);
    void shutterChanged(float shutter);
    void shutterTextChanged(QString shutterratio);
    void apertureChanged(float aperture);
    void apertureTextChanged(QString aperture);
    void focallenChanged(float focallen);
    void focallenTextChanged(QString focallen);
    void isoChanged(int iso);
    void isoTextChanged(QString iso);

private:

    QString m_make;
    QString m_model;
    QString m_lens;
    float   m_shutter;
    QString m_shutterText;
    float   m_aperture;
    QString m_apertureText;
    float   m_focallen;
    QString m_focallenText;
    int     m_iso;
    QString m_isoText;
};

#endif // CONFIGEXIF_H
