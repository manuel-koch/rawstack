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

public slots:

    bool add(TaskBuilderBase *builder);
    TaskBase *create(const std::string &name);

private:

    QThread                            *m_thread;
    QMap<std::string,TaskBuilderBase*>  m_builder;
};

#endif // TASKFACTORY_H
