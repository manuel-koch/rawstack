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

public slots:

    /// Add an instance that can create a task instance
    bool add(TaskBuilderBase *builder);

    /// Add an instance that can create a config instance
    bool add(ConfigBuilderBase *builder);

    /// Create config by name
    ConfigBase *create(QString name);

    /// Create task for given config
    TaskBase *create(ConfigBase *config);

private:

    QThread                         *m_thread;
    QMap<QString,TaskBuilderBase*>   m_taskBuilder;
    QMap<QString,ConfigBuilderBase*> m_configBuilder;
};

#endif // TASKFACTORY_H
