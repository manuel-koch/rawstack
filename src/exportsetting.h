#ifndef EXPORTSETTING_H
#define EXPORTSETTING_H

#include <QObject>

class ExportSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   config     READ config     WRITE setConfig     NOTIFY configChanged)
    Q_PROPERTY(QString   imgPath    READ imgPath    WRITE setImgPath    NOTIFY imgPathChanged)
    Q_PROPERTY(ImageType imgType    READ imgType    WRITE setImgType    NOTIFY imgTypeChanged)
    Q_PROPERTY(int       imgQuality READ imgQuality WRITE setImgQuality NOTIFY imgQualityChanged)
    Q_ENUMS(ImageType)

public:

    explicit ExportSetting(QObject *parent = 0);
    virtual ~ExportSetting();

    enum ImageType
    {
        JPG,
        TIF,
        PNG
    };

    QString   config() const { return m_config; }
    QString   imgPath() const { return m_imgPath; }
    ImageType imgType() const { return m_imgType; }
    int       imgQuality() const { return m_imgQuality; }

signals:

    void configChanged(QString config);
    void imgPathChanged(QString imgPath);
    void imgTypeChanged(ImageType imgType);
    void imgQualityChanged(int imgQuality);

public slots:

    void setConfig(QString config);
    void setImgPath(QString imgPath);
    void setImgType(ImageType imgType);
    void setImgQuality(int imgQuality);

private:

    QString adjustedImgPath(QString path);

private:

    QString   m_config;     // path to task stack configuration to be exported
    QString   m_imgPath;    // output path of exported image
    ImageType m_imgType;    // type of exported image
    int       m_imgQuality; // quality of exported image, i.e. JPG quality/compression
};

#endif // EXPORTSETTING_H