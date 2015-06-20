#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QObject>
#include <QMap>

class QThread;
class TaskBuilderBase;
class ConfigBuilderBase;
class TaskBase;
class ConfigBase;

class TaskFactory : public QObject
{
    Q_OBJECT

public:
    explicit TaskFactory(QObject *parent = 0);
    ~TaskFactory();

    static TaskFactory *getInstance() { return ms_instance; }
    static void setInstance( TaskFactory *instance ) { ms_instance = instance; }

public slots:

    /// Add an instance that can create a task instance, factory takes ownership of builder
    bool add(TaskBuilderBase *builder);

    /// Add an instance that can create a config instance, factory takes ownership of builder
    bool add(ConfigBuilderBase *builder);

    /// Create config by name
    ConfigBase *create(QString name);

    /// Create task for given config
    TaskBase *create(ConfigBase *config);

private:

    static TaskFactory *ms_instance;

    QThread                         *m_thread;        /// worker thread for tasks
    QMap<QString,TaskBuilderBase*>   m_taskBuilder;   /// map of registered task builders
    QMap<QString,ConfigBuilderBase*> m_configBuilder; /// map of registered config builders
};

#endif // TASKFACTORY_H
