#include "taskfactory.h"
#include "taskbuilder.h"
#include "configbuilder.h"
#include "taskstack.h"
#include "commontasks.h"
#include "taskbase.h"
#include "configbase.h"
#include "imagefactoryregistry.h"
#include "imageprovider.h"
#include "loghandler.h"
#include "exportqueue.h"
#include "exportimage.h"
#include "exportsetting.h"

#include "configbase.h"
#include "ufrawconfig.h"
#include "ufrawtask.h"
#include "rotateconfig.h"
#include "rotatetask.h"

#include <QDebug>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QImageReader>
#include <QByteArray>


void register_types()
{
    qRegisterMetaType<TaskBase*>("TaskBase");
    qRegisterMetaType<ConfigBase*>("ConfigBase");
    qRegisterMetaType<TaskStack*>("TaskStack");
    qRegisterMetaType<CommonTasks*>("CommonTasks");
    qRegisterMetaType<ImageFactory*>("ImageFactory");
    qRegisterMetaType<ImageFactoryBase*>("ImageFactoryBase");
    qRegisterMetaType<HistFactory*>("HistFactory");
    qmlRegisterType<TaskBase>("com.rawstack.types", 1, 0, "TaskBase");
    qRegisterMetaType<ExportSetting*>("ExportSetting");
    qRegisterMetaType<ExportImage*>("ExportImage");
    qRegisterMetaType<ExportQueue*>("ExportQueue");
}

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Magick::InitializeMagick( argv[0] );

    qSetMessagePattern("%{time} %{threadid} %{type}: %{message}");

    LogHandler logHandler;
    logHandler.start( "/Users/manuel/tmp/rawstack/debug.log" );

    register_types();

    qDebug() << "Supported image formats:";
    QList<QByteArray> imgFormats = QImageReader::supportedImageFormats();
    foreach( QByteArray fmt, imgFormats )
    {
        qDebug() << fmt.toStdString().c_str();
    }

    ImageFactoryRegistry imageFactoryRegistry;
    ImageFactoryRegistry::setInstance( &imageFactoryRegistry );

    TaskFactory taskFactory;
    TaskFactory::setInstance( &taskFactory );
    taskFactory.add( new ConfigBuilder<CommonConfig>("common") );
    taskFactory.add( new TaskBuilder<UfrawTask>("ufraw") );
    taskFactory.add( new ConfigBuilder<UfrawConfig>("ufraw") );
    taskFactory.add( new TaskBuilder<RotateTask>("rotate") );
    taskFactory.add( new ConfigBuilder<RotateConfig>("rotate") );

    QQmlImageProviderBase *imageProvider     = static_cast<QQmlImageProviderBase*>( new ImageProvider() );
    QString                imageProviderName = ImageProvider::name;

    ExportQueue exportQueue;

    TaskStack taskStack(true);
    if( argc > 1 )
        taskStack.loadFromFile( QUrl::fromLocalFile(argv[1]) );

    QQmlApplicationEngine engine;

    engine.addImageProvider( imageProviderName, imageProvider );

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty( "globalDevTaskStack", &taskStack );
    rootContext->setContextProperty( "globalExportQueue",  &exportQueue );

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    int result = app.exec();
    return result;
}
