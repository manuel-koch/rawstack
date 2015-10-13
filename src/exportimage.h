#ifndef EXPORTIMAGE_H
#define EXPORTIMAGE_H

#include <QObject>

class TaskStack;

class ExportImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString   config     READ config     WRITE setConfig       NOTIFY configChanged)
    Q_PROPERTY(int       imgQuality READ imgQuality WRITE setImgQuality   NOTIFY imgQualityChanged)
    Q_PROPERTY(ImageType imgType    READ imgType    WRITE setImgType      NOTIFY imgTypeChanged)
    Q_PROPERTY(QString   imgPath    READ imgPath    WRITE setImgPath      NOTIFY imgPathChanged)
    Q_PROPERTY(double    progress   READ progress                         NOTIFY progressChanged)
    Q_ENUMS(ImageType)

public:

    explicit ExportImage(QObject *parent = 0);
    virtual ~ExportImage();

    enum ImageType
    {
        JPG,
        TIF,
        PNG
    };

    QString   config() const;
    double    progress() const;

    int       imgQuality() const { return m_imgQuality; }
    ImageType imgType() const { return m_imgType; }
    QString   imgPath() const { return m_imgPath; }

signals:

    void configChanged(QString config);
    void progressChanged(double progress);

    void imgQualityChanged(int imgQuality);
    void imgTypeChanged(ImageType imgType);
    void imgPathChanged(QString imgPath);

public slots:

    void exportImage();

    void setConfig(QString config);
    void setImgQuality(int imgQuality);
    void setImgType(ImageType imgType);
    void setImgPath(QString imgPath);

private slots:

    void onDeveloped(bool developing);

private:

    TaskStack *m_stack;
    int        m_imgQuality;  // quality of exported image, i.e. JPG quality/compression
    ImageType  m_imgType;     // type of exported image
    QString    m_imgPath;     // output path of exported image
};

#endif // EXPORTIMAGE_H
