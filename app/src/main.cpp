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
#include "librawstack.h"
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
#include "exportimgtype.h"
#include "exporttemplate.h"
#include "exportsetting.h"
#include "configdb.h"
#include "configdbentry.h"
#include "configsetting.h"
#include "configsettingsstore.h"
#include "configexif.h"
#include "taskhistogram.h"
#include "menumodel.h"
#include "menuentry.h"

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
    qRegisterMetaType<MenuModel*>("MenuModel");
    qRegisterMetaType<MenuEntry*>("MenuEntry");
    qRegisterMetaType<TaskBase*>("TaskBase");
    qRegisterMetaType<TaskStack*>("TaskStack");
    qRegisterMetaType<CommonTasks*>("CommonTasks");
    qRegisterMetaType<ImageFactory*>("ImageFactory");
    qRegisterMetaType<ImageFactoryBase*>("ImageFactoryBase");
    qRegisterMetaType<ExportImgType*>("ExportImgType");
    qRegisterMetaType<ExportTemplate*>("ExportTemplate");
    qRegisterMetaType<ExportSetting*>("ExportSetting");
    qRegisterMetaType<ExportImage*>("ExportImage");
    qRegisterMetaType<ExportQueue*>("ExportQueue");
    qRegisterMetaType<ConfigDb*>("ConfigDb");
    qRegisterMetaType<ConfigDbEntry*>("ConfigDbEntry");
    qRegisterMetaType<ConfigSetting*>("ConfigSetting");
    qRegisterMetaType<ConfigSettingsStore*>("ConfigSettingsStore");
    qRegisterMetaType<ConfigExif*>("ConfigExif");
    qRegisterMetaType<ImageCacheGroup::Lifetime>("ImageCacheGroup::Lifetime");

    qmlRegisterUncreatableType<MenuModel>( "com.rawstack.types", 1, 0, "MenuModel", "Can't create type MenuModel" );
    qmlRegisterUncreatableType<ConfigSettingsStore>( "com.rawstack.types", 1, 0, "ConfigSettingsStore", "Can't create type ConfigSettingsStore" );
    qmlRegisterType<MenuEntry>( "com.rawstack.types", 1, 0, "MenuEntry" );
    qmlRegisterType<TaskBase>( "com.rawstack.types", 1, 0, "TaskBase" );
    qmlRegisterType<ConfigDbEntry>( "com.rawstack.types", 1, 0, "ConfigDbEntry" );
    qmlRegisterUncreatableType<ConfigDb>( "com.rawstack.types", 1, 0, "ConfigDb", "Can't create type ConfigDb" );
    qmlRegisterType<TaskHistogram>( "com.rawstack.types", 1, 0, "TaskHistogram" );
    qmlRegisterUncreatableType<ExportImgType>( "com.rawstack.types", 1, 0, "ExportImgType", "Can't create type ExportImgType" );
}

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Magick::InitializeMagick( argv[0] );

    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} %{threadid} %{type}: %{message} [%{file}:%{line}]");

    LogHandler logHandler;
    logHandler.start( "/Users/manuel/tmp/rawstack/debug.log" );

    LibRawstack::initResources();
    register_types();

    qDebug() << "Supported image formats:";
    QList<QByteArray> imgFormats = QImageReader::supportedImageFormats();
    foreach( QByteArray fmt, imgFormats )
    {
        qDebug() << fmt.toStdString().c_str();
    }

    MenuModel menuModel;
    MenuModel::setInstance( &menuModel );
    menuModel.screenOverview()->setChecked(true);

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
    ConfigSettingsStore configSettingsStore;
    ExportQueue exportQueue;
    ExportTemplate exportTemplate;
    TaskStack taskStack(true);

    if( argc > 1 )
        configDb.add( QUrl::fromLocalFile(argv[1]) );

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty( "globalMenuModel",      &menuModel );
    rootContext->setContextProperty( "globalConfigDb",       &configDb );
    rootContext->setContextProperty( "globalConfigStore",    &configSettingsStore );
    rootContext->setContextProperty( "globalDevTaskStack",   &taskStack );
    rootContext->setContextProperty( "globalExportTemplate", &exportTemplate );
    rootContext->setContextProperty( "globalExportQueue",    &exportQueue );

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    QList<QObject*> rootObjects = engine.rootObjects();
    if( rootObjects.size() )
        QMetaObject::invokeMethod(rootObjects[0], "centerOnScreen");
    else
        qWarning() << "Failed to create GUI";

    int result = app.exec();
    return result;
}
