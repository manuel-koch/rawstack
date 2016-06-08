#ifndef EXPORTTEMPLATE_H
#define EXPORTTEMPLATE_H

#include "exportimgtype.h"

#include <QObject>

class ExportTemplate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString                  templatePath READ templatePath WRITE setTemplatePath  NOTIFY templatePathChanged)
    Q_PROPERTY(QString                  imgPath      READ imgPath      WRITE setImgPath       NOTIFY imgPathChanged)
    Q_PROPERTY(ExportImgType::ImageType imgType      READ imgType      WRITE setImgType       NOTIFY imgTypeChanged)
    Q_PROPERTY(int                      imgQuality   READ imgQuality   WRITE setImgQuality    NOTIFY imgQualityChanged)

public:

    explicit ExportTemplate(QObject *parent = 0);

    QString templatePath() const { return m_templatePath; }
    QString imgPath() const { return m_imgPath; }
    ExportImgType::ImageType imgType() const { return m_imgType; }
    int imgQuality() const { return m_imgQuality; }

signals:

    void templatePathChanged(QString templatePath);
    void imgPathChanged(QString imgPath);
    void imgTypeChanged(ExportImgType::ImageType imgType);
    void imgQualityChanged(int imgQuality);

public slots:

    void setTemplatePath(QString templatePath);
    void setImgPath(QString imgPath);
    void setImgType(ExportImgType::ImageType imgType);
    void setImgQuality(int imgQuality);

private:

    QString                  m_templatePath; // image output path with variables
    QString                  m_imgPath;      // output path of exported image
    ExportImgType::ImageType m_imgType;      // type of exported image
    int                      m_imgQuality;   // quality of exported image, i.e. JPG quality/compression
};

#endif // EXPORTTEMPLATE_H
