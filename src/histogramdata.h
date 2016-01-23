/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#ifndef HISTOGRAMDATA_H
#define HISTOGRAMDATA_H

#include <QObject>
#include <QVariant>

class HistogramData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList red   READ red   NOTIFY redChanged)
    Q_PROPERTY(QVariantList green READ green NOTIFY greenChanged)
    Q_PROPERTY(QVariantList blue  READ blue  NOTIFY blueChanged)
    Q_PROPERTY(QVariantList lum   READ lum   NOTIFY lumChanged)

public:

    explicit HistogramData(QObject *parent = 0);

    const QVariantList &red() const { return m_red; }
    const QVariantList &green() const { return m_green; }
    const QVariantList &blue() const { return m_blue; }
    const QVariantList &lum() const { return m_lum; }

signals:

    void redChanged(QVariantList red);
    void greenChanged(QVariantList green);
    void blueChanged(QVariantList blue);
    void lumChanged(QVariantList lum);

public slots:

    void setRed( const QVariantList &red );
    void setGreen( const QVariantList &green );
    void setBlue( const QVariantList &blue );
    void setLum( const QVariantList &lum );

private:

    QVariantList m_red;
    QVariantList m_green;
    QVariantList m_blue;
    QVariantList m_lum;
};

#endif // HISTOGRAMDATA_H
