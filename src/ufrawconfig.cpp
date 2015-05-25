#include "ufrawconfig.h"

#include <QDebug>

UfrawConfig::UfrawConfig(QObject *parent)
    : ConfigBase(parent)
{
    // EMPTY
}

UfrawConfig::~UfrawConfig()
{
    // EMPTY
}

void UfrawConfig::setExposure(double exposure)
{
    if (m_exposure == exposure)
        return;
    m_exposure = exposure;
    qDebug() << "UfrawConfig::setExposure()" << m_exposure;
    emit exposureChanged(exposure);
    markDirty();
}

void UfrawConfig::setWbTemperature(int wbTemperature)
{
    if (m_wbTemperature == wbTemperature)
        return;
    m_wbTemperature = wbTemperature;
    qDebug() << "UfrawConfig::setWbTemperature()" << m_wbTemperature;
    emit wbTemperatureChanged(wbTemperature);
    markDirty();
}

void UfrawConfig::setWbGreen(double wbGreen)
{
    if (m_wbGreen == wbGreen)
        return;
    m_wbGreen = wbGreen;
    qDebug() << "UfrawConfig::setWbGreen()" << m_wbGreen;
    emit wbGreenChanged(wbGreen);
    markDirty();
}

