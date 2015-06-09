#ifndef COMMONTASKS_H
#define COMMONTASKS_H

#include <QObject>

class TaskBase;

class CommonTasks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TaskBase* ufraw READ ufraw NOTIFY ufrawChanged)
    Q_PROPERTY(TaskBase* final READ final NOTIFY finalChanged)

public:

    explicit CommonTasks(QObject *parent = 0);
    ~CommonTasks();

    TaskBase* ufraw() const { return m_ufraw; }
    TaskBase* final() const { return m_final; }

    void setUfraw(TaskBase *ufraw);
    void setFinal(TaskBase *final);

signals:

    void ufrawChanged(TaskBase* ufraw);
    void finalChanged(TaskBase* final);

private:

    TaskBase* m_ufraw;
    TaskBase* m_final;
};

#endif // COMMONTASK_H
