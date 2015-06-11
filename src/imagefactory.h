#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "imagefactorybase.h"
#include "imagefactoryunderexposed.h"
#include "imagefactoryoverexposed.h"

#include <QObject>

class ImageFactory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl image        READ image        NOTIFY imageChanged)
    Q_PROPERTY(QUrl underExposed READ underExposed NOTIFY underExposedChanged)
    Q_PROPERTY(QUrl overExposed  READ overExposed  NOTIFY overExposedChanged)

public:
    explicit ImageFactory(WorkerBase *worker, QObject *parent = NULL);
    ~ImageFactory();

    QUrl image()        const { return m_image.url(); }
    QUrl underExposed() const { return m_underExposed.url(); }
    QUrl overExposed()  const { return m_overExposed.url(); }

signals:

    void imageChanged(QUrl url);
    void underExposedChanged(QUrl url);
    void overExposedChanged(QUrl url);

private:

    ImageFactoryBase          m_image;
    ImageFactoryUnderExposed  m_underExposed;
    ImageFactoryOverExposed   m_overExposed;
};

#endif // IMAGEFACTORY_H
