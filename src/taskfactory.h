#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QObject>
#include <QMap>

class QThread;
class TaskBuilderBase;
class TaskBase;

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

    /// Create task of given name, worker of task will be hosted in given thread or default thread.
    /// Caller takes ownership of task.
    TaskBase *create(QString name, QThread *workerThread = NULL);

private:

    static TaskFactory *ms_instance;

    QThread                         *m_workerThread;  /// default worker thread for tasks
    QMap<QString,TaskBuilderBase*>   m_taskBuilder;   /// map of registered task builders
};

#endif // TASKFACTORY_H
