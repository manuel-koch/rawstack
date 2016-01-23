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
#include "exportsetting.h"
#include "configdbentry.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

ExportSetting::ExportSetting(ConfigDbEntry *config, QObject *parent)
    : QObject(parent)
    , m_config(config)
{
    if( m_config )
        setImgPath( m_config->config() );
}

ExportSetting::~ExportSetting()
{
    // EMPTY
}

void ExportSetting::setConfig(ConfigDbEntry *config)
{
    if( m_config == config )
        return;

    m_config = config;
    emit configChanged(m_config);

    if( m_config )
        setImgPath( m_config->config() );
}

void ExportSetting::setImgPath(QString imgPath)
{
    imgPath = adjustedImgPath( imgPath );
    if( m_imgPath == imgPath )
        return;

    m_imgPath = imgPath;
    qDebug() << "ExportSetting::setImgPath()" << m_imgPath;
    emit imgPathChanged(imgPath);
}

void ExportSetting::setImgType(ExportSetting::ImageType imgType)
{
    if( m_imgType == imgType )
        return;

    m_imgType = imgType;
    qDebug() << "ExportSetting::setImgType()" << m_imgType;
    emit imgTypeChanged(imgType);

    setImgPath( adjustedImgPath(m_imgPath) );
}

void ExportSetting::setImgQuality(int imgQuality)
{
    imgQuality = std::min( 100, std::max( 1, imgQuality) );
    if( m_imgQuality == imgQuality )
        return;

    m_imgQuality = imgQuality;
    qDebug() << "ExportSetting::setImgQuality()" << m_imgQuality;
    emit imgQualityChanged(imgQuality);
}

QString ExportSetting::adjustedImgPath(QString path)
{
    QFileInfo info( path );
    QString ext;
    switch( m_imgType )
    {
        case ExportSetting::TIF:
        {
            ext = ".tif";
            break;
        }
        case ExportSetting::PNG:
        {
            ext = ".png";
            break;
        }
        case ExportSetting::JPG:
        default:
        {
            ext = ".jpg";
            break;
        }
    }
    return QFileInfo( info.dir(), info.completeBaseName() + ext ).absoluteFilePath();
}

