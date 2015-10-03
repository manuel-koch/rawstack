#ifndef COMMONTASKS_H
#define COMMONTASKS_H

#include <QObject>
#include <QUrl>

class TaskBase;

class CommonTasks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TaskBase* ufraw READ ufraw NOTIFY ufrawChanged)
    Q_PROPERTY(TaskBase* final READ final NOTIFY finalChanged)
    Q_PROPERTY(QUrl      image READ image NOTIFY imageChanged)

public:

    explicit CommonTasks(QObject *parent = 0);
    ~CommonTasks();

    TaskBase* ufraw() const { return m_ufraw; }
    TaskBase* final() const { return m_final; }
    QUrl image() const;

    void setUfraw(TaskBase *ufraw);
    void setFinal(TaskBase *final);

signals:

    void ufrawChanged(TaskBase* ufraw);
    void finalChanged(TaskBase* final);
    void imageChanged(QUrl image);

private:

    TaskBase* m_ufraw;
    TaskBase* m_final;
};

#endif // COMMONTASK_H
