#include "leveltask.h"
#include "levelworker.h"
#include "levelsettings.h"

#include <QDebug>
#include <QThread>

LevelTask::LevelTask(QObject *parent, QThread *workerThread)
    : TaskBase(LevelSettings::Name,parent)
{
    qDebug() << "LevelTask::LevelTask()" << this << workerThread;
    setWorker( new LevelWorker() );
    worker()->moveToThread( workerThread );
}

LevelTask::~LevelTask()
{
    // EMPTY
}

void LevelTask::initTaskSettings()
{
    ConfigSettings *settings = config()->settings();

    m_blackpoint = settings->getSetting( LevelSettings::Blackpoint );
    m_blackpoint->initIfNull( LevelSettings::DefaultBlackpoint );

    m_midpoint = settings->getSetting( LevelSettings::Midpoint );
    m_midpoint->initIfNull( LevelSettings::DefaultMidpoint );

    m_whitepoint = settings->getSetting( LevelSettings::Whitepoint );
    m_whitepoint->initIfNull( LevelSettings::DefaultWhitepoint );

    m_brightness = settings->getSetting( LevelSettings::Brightness );
    m_brightness->initIfNull( LevelSettings::DefaultBrightness );

    m_saturation = settings->getSetting( LevelSettings::Saturation );
    m_saturation->initIfNull( LevelSettings::DefaultSaturation );
}
