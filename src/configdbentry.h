#ifndef CONFIGDBENTRY_H
#define CONFIGDBENTRY_H

#include "imagefactorythumbnail.h"
#include "imagefactoryfinal.h"
#include "configsettings.h"

#include <Magick++.h>

#include <QObject>
#include <QUrl>
#include <QDomNode>

class ConfigDb;

class ConfigDbEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString         title     READ title                     NOTIFY titleChanged)
    Q_PROPERTY(QString         raw       READ raw       WRITE setRaw    NOTIFY rawChanged)
    Q_PROPERTY(QString         config    READ config    WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(ConfigSettings* settings  READ settings  CONSTANT)
    Q_PROPERTY(QUrl            thumbnail READ thumbnail                 NOTIFY thumbnailChanged)
    Q_PROPERTY(QUrl            final     READ final                     NOTIFY finalChanged)

public:

    explicit ConfigDbEntry(QObject *parent = 0);

    bool equals( const ConfigDbEntry *other );

    QString title() const { return m_title; }
    QString raw() const { return m_raw; }
    QString config() const { return m_config; }
    ConfigSettings *settings() { return &m_settings; }
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
    void rawChanged(QString raw);
    void configChanged(QString config);
    void thumbnailChanged(QUrl thumbnail);
    void finalChanged(QUrl final);

    /// Request to add a duplicate of current configuration to database
    void duplicate();

    /// Request to remove current configuration from database
    void remove();

    void aboutToBeDestroyed();

public slots:

    void setRaw(QString raw);
    void setConfig(QString config);

private:

    void setTitle(QString title);

private:

    QString               m_title;     // title/name of configuration ( basename of configuration path )
    QString               m_raw;       // path to RAW image
    QString               m_config;    // path to rawstack configuration file

    ConfigSettings        m_settings;  // settings for the tasks to develop the RAW image

    ImageFactoryThumbnail m_thumbnail; // factory for thumbnail of preview / developed image
    ImageFactoryFinal     m_final;     // factory for full size of preview / developed image
};

#endif // CONFIGDBENTRY_H
