/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#include "taskfactory.h"
#include "taskbuilder.h"
#include "taskstack.h"
#include "commontasks.h"
#include "taskbase.h"
#include "imagefactoryregistry.h"
#include "imageprovider.h"
#include "loghandler.h"
#include "exportqueue.h"
#include "exportimage.h"
#include "exportsetting.h"
#include "configdb.h"
#include "configdbentry.h"
#include "configsetting.h"
#include "configexif.h"
#include "taskhistogram.h"

#include "ufrawtask.h"
#include "ufrawsettings.h"
#include "leveltask.h"
#include "levelsettings.h"
#include "rotatetask.h"
#include "rotatesettings.h"
#include "sharpentask.h"
#include "sharpensettings.h"

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
    qRegisterMetaType<TaskStack*>("TaskStack");
    qRegisterMetaType<CommonTasks*>("CommonTasks");
    qRegisterMetaType<ImageFactory*>("ImageFactory");
    qRegisterMetaType<ImageFactoryBase*>("ImageFactoryBase");
    qRegisterMetaType<ExportSetting*>("ExportSetting");
    qRegisterMetaType<ExportImage*>("ExportImage");
    qRegisterMetaType<ExportQueue*>("ExportQueue");
    qRegisterMetaType<ConfigDb*>("ConfigDb");
    qRegisterMetaType<ConfigDbEntry*>("ConfigDbEntry");
    qRegisterMetaType<ConfigSetting*>("ConfigSetting");
    qRegisterMetaType<ConfigExif*>("ConfigExif");
    qRegisterMetaType<ImageCacheGroup::Lifetime>("ImageCacheGroup::Lifetime");

    qmlRegisterType<TaskBase>( "com.rawstack.types", 1, 0, "TaskBase" );
    qmlRegisterType<ConfigDbEntry>( "com.rawstack.types", 1, 0, "ConfigDbEntry" );
    qmlRegisterUncreatableType<ConfigDb>( "com.rawstack.types", 1, 0, "ConfigDb", "Can't create type ConfigDb" );
    qmlRegisterType<TaskHistogram>( "com.rawstack.types", 1, 0, "TaskHistogram" );
}

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Magick::InitializeMagick( argv[0] );

    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} %{threadid} %{type}: %{message}");

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
    taskFactory.add( new TaskBuilder<UfrawTask>(UfrawSettings::Name) );
    taskFactory.add( new TaskBuilder<LevelTask>(LevelSettings::Name) );
    taskFactory.add( new TaskBuilder<RotateTask>(RotateSettings::Name) );
    taskFactory.add( new TaskBuilder<SharpenTask>(SharpenSettings::Name) );

    QQmlImageProviderBase *imageProvider     = static_cast<QQmlImageProviderBase*>( new ImageProvider() );
    QString                imageProviderName = ImageProvider::name;

    QQmlApplicationEngine engine;
    engine.addImageProvider( imageProviderName, imageProvider );

    ConfigDb configDb;
    ExportQueue exportQueue;
    TaskStack taskStack(true);

    if( argc > 1 )
        configDb.add( QUrl::fromLocalFile(argv[1]) );

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty( "globalConfigDb",     &configDb );
    rootContext->setContextProperty( "globalDevTaskStack", &taskStack );
    rootContext->setContextProperty( "globalExportQueue",  &exportQueue );

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    QList<QObject*> rootObjects = engine.rootObjects();
    if( rootObjects.size() )
        QMetaObject::invokeMethod(rootObjects[0], "centerOnScreen");

    int result = app.exec();
    return result;
}
