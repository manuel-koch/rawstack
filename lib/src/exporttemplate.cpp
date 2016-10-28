#include "exporttemplate.h"

ExportTemplate::ExportTemplate(QObject *parent)
    : QObject(parent)
    , m_imgPath("$(rawdir)/exported/$(rawname).$(ext)")
    , m_imgType(ExportImgType::JPG)
    , m_imgQuality(95)
{
    // EMPTY
}

void ExportTemplate::setImgPath(QString imgPath)
{
    if (m_imgPath == imgPath)
        return;

    m_imgPath = imgPath;
    emit imgPathChanged(imgPath);
}

void ExportTemplate::setImgType(ExportImgType::ImageType imgType)
{
    if (m_imgType == imgType)
        return;

    m_imgType = imgType;
    emit imgTypeChanged(imgType);
}

void ExportTemplate::setImgQuality(int imgQuality)
{
    if (m_imgQuality == imgQuality)
        return;

    m_imgQuality = imgQuality;
    emit imgQualityChanged(imgQuality);
}
