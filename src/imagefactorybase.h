#ifndef IMAGEFACTORYNORMAL_H
#define IMAGEFACTORYNORMAL_H

#include <QObject>
#include <QUrl>
#include <QImage>

class WorkerBase;

class ImageFactoryBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)

public:

    explicit ImageFactoryBase(WorkerBase *worker);
    virtual ~ImageFactoryBase();

    QString id() const { return m_id; }
    QUrl    url() const { return m_url; }
    virtual QImage image();

signals:

    void urlChanged(QUrl url);

private slots:

    void onCycleChanged(int cycle);

protected:

    WorkerBase *m_worker;

private:

    QString    m_id;
    QUrl       m_url;
};

#endif // IMAGEFACTORYNORMAL_H
