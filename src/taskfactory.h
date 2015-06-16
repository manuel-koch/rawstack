#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QObject>
#include <QMap>

class QThread;
class TaskBuilderBase;
class TaskBase;
class ConfigBase;

class TaskFactory : public QObject
{
    Q_OBJECT

public:
    explicit TaskFactory(QObject *parent = 0);
    ~TaskFactory();

public slots:

    /// Add an instance that can create a task
    bool add(TaskBuilderBase *builder);

    // Create task for given config
    TaskBase *create(ConfigBase *config);

private:

    QThread                        *m_thread;
    QMap<QString,TaskBuilderBase*>  m_builder;
};

#endif // TASKFACTORY_H
