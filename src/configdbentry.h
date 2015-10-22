#ifndef CONFIGDBENTRY_H
#define CONFIGDBENTRY_H

#include "imagefactorythumbnail.h"

#include <QObject>
#include <QUrl>

class ConfigDbEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title     READ title                        NOTIFY titleChanged)
    Q_PROPERTY(QString raw       READ raw       WRITE setRaw       NOTIFY rawChanged)
    Q_PROPERTY(QString config    READ config    WRITE setConfig    NOTIFY configChanged)
    Q_PROPERTY(QUrl    thumbnail READ thumbnail                    NOTIFY thumbnailChanged)

public:

    explicit ConfigDbEntry(QObject *parent = 0);

    bool equals( const ConfigDbEntry *other );

    QString title() const { return m_title; }
    QString raw() const { return m_raw; }
    QString config() const { return m_config; }
    QUrl    thumbnail() const { return m_thumbnail.url(); }

    bool isValidRaw() const;

signals:

    void titleChanged(QString title);
    void rawChanged(QString raw);
    void configChanged(QString config);
    void thumbnailChanged(QUrl thumbnail);

public slots:

    void setRaw(QString raw);
    void setConfig(QString config);

private:

    void setTitle(QString title);

private:

    QString               m_title;     // title/name of configuration
    QString               m_raw;       // path to RAW image
    QString               m_config;    // path to rawstack configuration file
    ImageFactoryThumbnail m_thumbnail; // factory for thumbnail of preview / developed image
};

#endif // CONFIGDBENTRY_H
