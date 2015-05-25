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

