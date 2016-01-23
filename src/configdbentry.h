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
#ifndef CONFIGDBENTRY_H
#define CONFIGDBENTRY_H

#include "imagefactorythumbnail.h"
#include "imagefactoryfinal.h"
#include "configsettings.h"
#include "configexif.h"

#include <Magick++.h>

#include <QObject>
#include <QUrl>
#include <QDomNode>

class ConfigDb;

class ConfigDbEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString         title     READ title                     NOTIFY titleChanged)
    Q_PROPERTY(int             instance  READ instance                  NOTIFY instanceChanged)
    Q_PROPERTY(QString         raw       READ raw       WRITE setRaw    NOTIFY rawChanged)
    Q_PROPERTY(QString         config    READ config    WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(ConfigSettings* settings  READ settings  CONSTANT)
    Q_PROPERTY(ConfigExif*     exif      READ exif      CONSTANT)
    Q_PROPERTY(QUrl            thumbnail READ thumbnail                 NOTIFY thumbnailChanged)
    Q_PROPERTY(QUrl            final     READ final                     NOTIFY finalChanged)

public:

    explicit ConfigDbEntry(QObject *parent = 0);

    bool equals( const ConfigDbEntry *other );
    int  compare( const ConfigDbEntry *other );

    QString title() const { return m_title; }
    int instance() const { return m_instance; }
    QString raw() const { return m_raw; }
    QString config() const { return m_config; }
    ConfigSettings *settings() { return &m_settings; }
    ConfigExif *exif() { return &m_exif; }
    QUrl    thumbnail() const { return m_thumbnail.url(); }
    QUrl    final() const { return m_final.url(); }

    bool isValidRaw() const;

    ConfigDb *db() const;

    void load(QString path = "");
    void save(QString path = "");
    void fromXML(QDomDocument const &doc );
    void toXML(QDomDocument &doc) const;

signals:

    void titleChanged(QString title);
    void instanceChanged(int instance);
    void rawChanged(QString raw);
    void configChanged(QString config);
    void thumbnailChanged(QUrl thumbnail);
    void finalChanged(QUrl final);

    /// Request to add a duplicate of current configuration to database
    void duplicate();

    /// Request to remove current configuration from database
    void remove();

    /// Request to remove current configuration from database and delete the configuration on filesystem
    void purge();

    void aboutToBeDestroyed();

public slots:

    void setRaw(QString raw);
    void setConfig(QString config);
    void loadExif();

private:

    void setTitle(QString title);
    void setInstance(int instance);

private:

    QString               m_title;     // title/name of configuration ( basename of configuration path )
    int                   m_instance;  // instance of configuration ( multiple configurations can use the same RAW )
    QString               m_raw;       // path to RAW image
    QString               m_config;    // path to rawstack configuration file

    ConfigSettings        m_settings;  // settings for the tasks to develop the RAW image
    ConfigExif            m_exif;      // EXIF info of RAW image

    ImageFactoryThumbnail m_thumbnail; // factory for thumbnail of preview / developed image
    ImageFactoryFinal     m_final;     // factory for full size of preview / developed image
};

#endif // CONFIGDBENTRY_H
