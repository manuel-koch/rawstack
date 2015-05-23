#ifndef TASKBUILDERBASE
#define TASKBUILDERBASE

#include <QObject>

class QThread;
class TaskBase;

class TaskBuilderBase
{
public:

    explicit TaskBuilderBase(const std::string &name) : m_name(name) { }
    virtual ~TaskBuilderBase() { }

    const std::string & name() { return m_name; }

    virtual TaskBase *create(QObject *parent, QThread *thread) = 0;

private:

    std::string m_name;
};

#endif // TASKBUILDERBASE

