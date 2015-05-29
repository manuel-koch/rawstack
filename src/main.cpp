#include "taskfactory.h"
#include "taskbuilder.h"
#include "taskstack.h"
#include "taskbase.h"
#include "configbase.h"

// FIXME: Remove temporary includes
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
    qmlRegisterType<TaskBase>("com.rawstack.types", 1, 0, "TaskBase");
}

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Magick::InitializeMagick( argv[0] );

    register_types();
    qSetMessagePattern("%{time} %{threadid} %{type}: %{message}");

    TaskFactory factory;
    TaskBuilder<UfrawTask> ufrawBuilder("ufraw");
    factory.add( &ufrawBuilder );

    TaskBase *task = factory.create("ufraw");

    TaskStack taskStack;
    taskStack.addTask( task );

    //task->develop();

    QQmlApplicationEngine engine;
    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty( "theDevTaskStack", &taskStack );

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    return app.exec();
}
