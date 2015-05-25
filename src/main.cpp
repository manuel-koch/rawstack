#include "taskfactory.h"
#include "taskbuilder.h"
#include "ufrawtask.h"

#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Magick::InitializeMagick( argv[0] );

    qSetMessagePattern("%{time} %{threadid} %{type}: %{message}");

    TaskFactory factory;
    TaskBuilder<UfrawTask> ufrawBuilder("ufraw");
    factory.add( &ufrawBuilder );
    TaskBase *task = factory.create("ufraw");
    task->develop();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    return app.exec();
}
