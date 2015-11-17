#include "sharpenworker.h"
#include "sharpensettings.h"
#include "configdbentry.h"
#include "configsetting.h"

#include <math.h>

#include <QDebug>
#include <QThread>

SharpenWorker::SharpenWorker()
    : WorkerBase("sharpen")
{
    // EMPTY
}

SharpenWorker::~SharpenWorker()
{
    // EMPTY
}

void SharpenWorker::registerSettingsImpl()
{
    hashSetting(SharpenSettings::Radius);
    hashSetting(SharpenSettings::Sigma);
}

void SharpenWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    Q_UNUSED(preview);

    qDebug() << "SharpenWorker::developImpl()" << this << predecessor;

    double radius = config()->settings()->getSetting(SharpenSettings::Radius)->value().toDouble();
    double sigma  = config()->settings()->getSetting(SharpenSettings::Sigma)->value().toDouble();
    if( radius>=0 && sigma>0 )
    {
        qDebug() << "SharpenWorker::developImpl()" << this << radius << sigma;
        m_img.sharpen( radius, sigma );
    }
}
