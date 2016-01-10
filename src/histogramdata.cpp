#include "histogramdata.h"

#include <QDebug>

HistogramData::HistogramData(QObject *parent)
    : QObject(parent)
{
    // EMPTY
}

void HistogramData::setRed(const QVariantList &red)
{
    if( red == m_red )
        return;
    m_red = red;
    emit redChanged( m_red );
}

void HistogramData::setGreen(const QVariantList &green)
{
    if( green == m_green )
        return;
    m_green = green;
    emit greenChanged( m_green );
}

void HistogramData::setBlue(const QVariantList &blue)
{
    if( blue == m_blue )
        return;
    m_blue = blue;
    emit blueChanged( m_blue );
}

void HistogramData::setLum(const QVariantList &lum)
{
    if( lum == m_lum )
        return;
    m_lum = lum;
    emit lumChanged( m_lum );
}
