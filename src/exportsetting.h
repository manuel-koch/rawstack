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
#ifndef EXPORTSETTING_H
#define EXPORTSETTING_H

#include "exportimgtype.h"

#include <QObject>

class ConfigDbEntry;

class ExportSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConfigDbEntry*           config     READ config     WRITE setConfig     NOTIFY configChanged)
    Q_PROPERTY(QString                  imgPath    READ imgPath    WRITE setImgPath    NOTIFY imgPathChanged)
    Q_PROPERTY(ExportImgType::ImageType imgType    READ imgType    WRITE setImgType    NOTIFY imgTypeChanged)
    Q_PROPERTY(int                      imgQuality READ imgQuality WRITE setImgQuality NOTIFY imgQualityChanged)

public:

    explicit ExportSetting(ConfigDbEntry *config = 0, QObject *parent = 0);
    virtual ~ExportSetting();

    ConfigDbEntry           *config() const { return m_config; }
    QString                  imgPath() const { return m_imgPath; }
    ExportImgType::ImageType imgType() const { return m_imgType; }
    int                      imgQuality() const { return m_imgQuality; }

signals:

    void configChanged(ConfigDbEntry *config);
    void imgPathChanged(QString imgPath);
    void imgTypeChanged(ExportImgType::ImageType imgType);
    void imgQualityChanged(int imgQuality);

public slots:

    void setConfig(ConfigDbEntry *config);
    void setImgPath(QString imgPath);
    void setImgType(ExportImgType::ImageType imgType);
    void setImgQuality(int imgQuality);

private:

    QString resolveVars( QString txt );

private:

    ConfigDbEntry           *m_config;     // path to task stack configuration to be exported
    QString                  m_imgPath;    // output path of exported image
    ExportImgType::ImageType m_imgType;    // type of exported image
    int                      m_imgQuality; // quality of exported image, i.e. JPG quality/compression
};

#endif // EXPORTSETTING_H
