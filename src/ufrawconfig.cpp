#include "ufrawconfig.h"

#include <QDebug>

const char *UfrawConfig::Name = "ufraw";

UfrawConfig::UfrawConfig(QObject *parent)
    : ConfigBase(UfrawConfig::Name,parent)
    , m_exposure(UfrawConfig::DefaultExposure)
    , m_wbTemperature(UfrawConfig::DefaultWbTemperature)
    , m_wbGreen(UfrawConfig::DefaultWbGreen)
{
    setCanDisable(false);
}

UfrawConfig::~UfrawConfig()
{
    // EMPTY
}

QJsonObject UfrawConfig::toJson() const
{
    qDebug() << "UfrawConfig::toJson()";
    QJsonObject json( ConfigBase::toJson() );
    QJsonObject settings;
    settings["exposure"]      = m_exposure;
    settings["wbTemperature"] = m_wbTemperature;
    settings["wbGreen"]       = m_wbGreen;
    json["ufrawSettings"] = settings;
    return json;
}

bool UfrawConfig::fromJson(const QJsonObject &json)
{
    qDebug() << "UfrawConfig::fromJson()";
    ConfigBase::fromJson(json);
    if( name() != Name )
        return false;

    auto it = json.find("ufrawSettings");
    if( it == json.end() )
        return false;

    QJsonObject settings = it.value().toObject();

    if( settings["exposure"].isUndefined() )
        setExposure( DefaultExposure );
    else
        setExposure( settings["exposure"].toDouble() );

    if( settings["wbTemperature"].isUndefined() )
        setWbTemperature( DefaultWbTemperature );
    else
        setWbTemperature( settings["wbTemperature"].toInt() );

    if( settings["wbGreen"].isUndefined() )
        setWbGreen( DefaultWbGreen );
    else
        setWbGreen( settings["wbGreen"].toDouble() );

    resetDirty();

    return true;
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

void UfrawConfig::setRaw(QString raw)
{
    if (m_raw == raw)
        return;
    m_raw = raw;
    qDebug() << "UfrawConfig::setRaw()" << m_raw;
    emit rawChanged(raw);
    markDirty();
}

