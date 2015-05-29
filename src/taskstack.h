#ifndef TASKSTACK_H
#define TASKSTACK_H

#include <QAbstractListModel>

class TaskBase;

class TaskStack : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)

public:

    typedef QHash<int, QByteArray> RoleMap;

    enum Roles
    {
        NameRole = Qt::UserRole+1,
        TaskRole,
        ConfigRole,
        ProgressRole
    };

    explicit TaskStack( QObject *parent = NULL );
    virtual ~TaskStack();

    void addTask(TaskBase *task, int idx = -1 );
    double progress() const { return m_progress; }

public slots:

    Q_INVOKABLE void develop();
    void setProgress(double progress);

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

signals:

    void progressChanged(double progress);

private slots:

    void onTaskStarted();
    void onTaskProgress(double progress);
    void onTaskFinished();

private:

    RoleMap           m_rolemap;
    QList<TaskBase*>  m_tasks;
    double            m_progress;
};

#endif // TASKSTACK_H
