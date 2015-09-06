#include "taskfactory.h"
#include "taskbuilder.h"
#include "configbuilder.h"
#include "taskstack.h"
#include "commontasks.h"
#include "taskbase.h"
#include "configbase.h"
#include "imagefactoryregistry.h"
#include "imageprovider.h"

// FIXME: Remove temporary includes
#include "ufrawconfig.h"
#include "ufrawtask.h"
#include "configbase.h"
// END-OF-FIXME

#include <QDebug>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>

void register_types()
{
    qRegisterMetaType<TaskBase*>("TaskBase");
    qRegisterMetaType<ConfigBase*>("ConfigBase");
    qRegisterMetaType<TaskStack*>("TaskStack");
    qRegisterMetaType<CommonTasks*>("CommonTasks");
    qRegisterMetaType<ImageFactory*>("ImageFactory");
    qRegisterMetaType<ImageFactoryBase*>("ImageFactoryBase");
    qmlRegisterType<TaskBase>("com.rawstack.types", 1, 0, "TaskBase");
}

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Magick::InitializeMagick( argv[0] );

    register_types();
    qSetMessagePattern("%{time} %{threadid} %{type}: %{message}");

    ImageFactoryRegistry imageFactoryRegistry;
    ImageFactoryRegistry::setInstance( &imageFactoryRegistry );

    TaskFactory taskFactory;
    TaskFactory::setInstance( &taskFactory );
    taskFactory.add( new TaskBuilder<UfrawTask>("ufraw") );
    taskFactory.add( new ConfigBuilder<UfrawConfig>("ufraw") );

    QQmlImageProviderBase *imageProvider     = static_cast<QQmlImageProviderBase*>( new ImageProvider() );
    QString                imageProviderName = ImageProvider::name;

    TaskStack taskStack;
    if( argc > 1 )
        taskStack.loadFromFile( QUrl::fromLocalFile(argv[1]) );

    QQmlApplicationEngine engine;

    engine.addImageProvider( imageProviderName, imageProvider );

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty( "globalDevTaskStack", &taskStack );

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    int result = app.exec();
    return result;
}
