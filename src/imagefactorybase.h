#ifndef IMAGEFACTORYBASE_H
#define IMAGEFACTORYBASE_H

#include <QObject>
#include <QUrl>
#include <QImage>

class WorkerBase;

class ImageFactoryBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)

public:

    explicit ImageFactoryBase();
    virtual ~ImageFactoryBase();

    QString id() const { return m_id; }
    QUrl    url() const { return m_url; }
    virtual QImage image();

signals:

    void urlChanged(QUrl url);

protected:

    void setUrl(int hash);
    void setUrl(const QByteArray &hash);

private:

    QString    m_id;
    QUrl       m_url;
};

#endif // IMAGEFACTORYBASE_H
