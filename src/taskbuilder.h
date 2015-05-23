#ifndef TASKBUILDER_H
#define TASKBUILDER_H

#include "taskbuilderbase.h"

class QObject;
class QThread;

template <typename T>
class TaskBuilder : public TaskBuilderBase
{
public:
    explicit TaskBuilder(const std::string &name) : TaskBuilderBase(name) { }
    virtual ~TaskBuilder() { }

    virtual TaskBase *create(QObject *parent, QThread *thread)
    {
        return new T(parent,thread);
    }
};

#endif // TASKBUILDER_H
