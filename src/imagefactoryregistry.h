#ifndef IMAGEFACTORYREGISTRY_H
#define IMAGEFACTORYREGISTRY_H

#include <QObject>
#include <QMap>

class ImageFactoryBase;

class ImageFactoryRegistry : public QObject
{
    Q_OBJECT

public:

    explicit ImageFactoryRegistry(QObject *parent = NULL);
    ~ImageFactoryRegistry();

    static ImageFactoryRegistry *getInstance() { return ms_instance; }
    static void setInstance( ImageFactoryRegistry *instance ) { ms_instance = instance; }

    void registerFactory(ImageFactoryBase *factory);
    void unregisterFactory(ImageFactoryBase *factory);

    ImageFactoryBase *getFactory(QString id);

private:

    static ImageFactoryRegistry *ms_instance;

    QMap<QString,ImageFactoryBase*> m_factories;
};

#endif // IMAGEFACTORYREGISTRY_H
