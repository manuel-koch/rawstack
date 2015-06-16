#ifndef TASKBUILDERBASE
#define TASKBUILDERBASE

#include <QObject>

class QThread;
class TaskBase;

class TaskBuilderBase
{
public:

    explicit TaskBuilderBase(QString name) : m_name(name) { }
    virtual ~TaskBuilderBase() { }

    QString name() { return m_name; }

    virtual TaskBase *create(QObject *parent, QThread *thread) = 0;

private:

    QString m_name;
};

#endif // TASKBUILDERBASE

