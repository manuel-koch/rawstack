#include "levelworker.h"
#include "levelsettings.h"
#include "configdbentry.h"
#include "configsetting.h"

#include <math.h>

#include <QDebug>
#include <QThread>

LevelWorker::LevelWorker()
    : WorkerBase(LevelSettings::Name)
{
    // EMPTY
}

LevelWorker::~LevelWorker()
{
    // EMPTY
}

void LevelWorker::registerSettingsImpl()
{
    hashSetting(LevelSettings::Blackpoint);
    hashSetting(LevelSettings::Midpoint);
    hashSetting(LevelSettings::Whitepoint);
    hashSetting(LevelSettings::Brightness);
    hashSetting(LevelSettings::Saturation);
}

void LevelWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    Q_UNUSED(preview);

    qDebug() << "LevelWorker::developImpl()" << this << predecessor;

    double blackpoint = config()->settings()->getSetting(LevelSettings::Blackpoint)->value().toDouble();
    double midpoint   = config()->settings()->getSetting(LevelSettings::Midpoint)->value().toDouble();
    double whitepoint = config()->settings()->getSetting(LevelSettings::Whitepoint)->value().toDouble();
    double brightness = config()->settings()->getSetting(LevelSettings::Brightness)->value().toDouble();
    double saturation = config()->settings()->getSetting(LevelSettings::Saturation)->value().toDouble();

    if( blackpoint != LevelSettings::DefaultBlackpoint ||
        midpoint   != LevelSettings::DefaultMidpoint   ||
        whitepoint != LevelSettings::DefaultWhitepoint    )
    {
        qDebug() << "LevelWorker::developImpl() level" << this << blackpoint << midpoint << whitepoint;
        double b = blackpoint*MaxRGB;
        double m = midpoint <= 0.5 ? midpoint / 0.5 : 1 + (midpoint - 0.5) * 10;
        double w = whitepoint*MaxRGB;
        m_img.level( b, w, m );
        setProgress(0.5);
    }

    if( brightness != LevelSettings::DefaultBrightness ||
        saturation != LevelSettings::DefaultSaturation    )
    {
        qDebug() << "LevelWorker::developImpl() modulate" << this << brightness << saturation;
        m_img.modulate( brightness*100, saturation*100, 100 );
    }
}
