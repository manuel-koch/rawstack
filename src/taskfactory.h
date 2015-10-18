#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QObject>
#include <QMap>

class QThread;
class TaskBuilderBase;
class ConfigBuilderBase;
class TaskBase;
class ConfigBase;
class ImageCacheBase;

class TaskFactory : public QObject
{
    Q_OBJECT

public:
    explicit TaskFactory(QObject *parent = 0);
    ~TaskFactory();

    static TaskFactory *getInstance() { return ms_instance; }
    static void setInstance( TaskFactory *instance ) { ms_instance = instance; }

public slots:

    /// Add an instance that can create a task instance, factory takes ownership of builder.
    bool add(TaskBuilderBase *builder);

    /// Add an instance that can create a config instance, factory takes ownership of builder.
    bool add(ConfigBuilderBase *builder);

    /// Create config by name, caller takes owership of configuration.
    ConfigBase *create(QString name);

    /// Create task for given config, worker of task will be hosted in given task or default task.
    /// Caller takes ownership of task.
    TaskBase *create(ConfigBase *config, QThread *workerThread = NULL);

private:

    static TaskFactory *ms_instance;

    QThread                         *m_workerThread;  /// default worker thread for tasks
    QMap<QString,TaskBuilderBase*>   m_taskBuilder;   /// map of registered task builders
    QMap<QString,ConfigBuilderBase*> m_configBuilder; /// map of registered config builders
    ImageCacheBase                  *m_imgCache;      /// caching generated images of workers
};

#endif // TASKFACTORY_H
