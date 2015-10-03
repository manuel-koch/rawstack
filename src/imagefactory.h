#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "imagefactoryworker.h"
#include "imagefactorypreview.h"
#include "imagefactoryunderexposed.h"
#include "imagefactoryoverexposed.h"
#include "histfactory.h"

#include <QObject>

class ImageFactory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl         image        READ image        NOTIFY imageChanged)
    Q_PROPERTY(QUrl         preview      READ preview      NOTIFY previewChanged)
    Q_PROPERTY(QUrl         underExposed READ underExposed NOTIFY underExposedChanged)
    Q_PROPERTY(QUrl         overExposed  READ overExposed  NOTIFY overExposedChanged)
    Q_PROPERTY(HistFactory* histogram    READ histogram    CONSTANT)

public:
    explicit ImageFactory(WorkerBase *worker, QObject *parent = NULL);
    ~ImageFactory();

    QUrl image()        const { return m_image.url(); }
    QUrl preview()      const { return m_preview.url(); }
    QUrl underExposed() const { return m_underExposed.url(); }
    QUrl overExposed()  const { return m_overExposed.url(); }
    HistFactory *histogram() { return &m_histFactory; }

signals:

    void imageChanged(QUrl url);
    void previewChanged(QUrl url);
    void underExposedChanged(QUrl url);
    void overExposedChanged(QUrl url);

private:

    ImageFactoryWorker        m_image;
    ImageFactoryPreview       m_preview;
    ImageFactoryUnderExposed  m_underExposed;
    ImageFactoryOverExposed   m_overExposed;
    HistFactory               m_histFactory;
};

#endif // IMAGEFACTORY_H
