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
    taskFactory.add( new TaskBuilder<UfrawTask>("ufraw") );
    taskFactory.add( new ConfigBuilder<UfrawConfig>("ufraw") );

    ConfigBase *ufrawConfig = taskFactory.create("ufraw");
    TaskBase *task = taskFactory.create(ufrawConfig);
    task->config()->setProperty( "raw", "/Users/manuel/tmp/TestBilder/01.cr2" );
    TaskStack taskStack;
    taskStack.addTask( task );
    taskStack.saveToFile("/Users/manuel/tmp/TestBilder/01.rawstack");

    QQmlApplicationEngine engine;
    engine.addImageProvider( ImageProvider::name, static_cast<QQmlImageProviderBase*>( new ImageProvider() ) );
    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty( "globalDevTaskStack", &taskStack );

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    int result = app.exec();
    return result;
}
