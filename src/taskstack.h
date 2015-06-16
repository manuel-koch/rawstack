#ifndef TASKSTACK_H
#define TASKSTACK_H

#include <QAbstractListModel>

class TaskBase;
class CommonTasks;

class TaskStack : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool         developing READ developing NOTIFY developingChanged)
    Q_PROPERTY(double       progress   READ progress  NOTIFY progressChanged)
    Q_PROPERTY(CommonTasks* tasks      READ tasks     CONSTANT)
    Q_PROPERTY(QString      config     READ config    NOTIFY configChanged)

public:

    typedef QHash<int, QByteArray> RoleMap;

    enum Roles
    {
        NameRole = Qt::UserRole+1,
        TaskRole,
        ConfigRole,
        Developing,
        ProgressRole
    };

    explicit TaskStack( QObject *parent = NULL );
    virtual ~TaskStack();

    void addTask(TaskBase *task, int idx = -1 );
    bool developing() const { return m_developing; }
    double progress() const { return m_progress; }
    QString config() const { return m_config; }

public slots:

    /// Start developing the image with stacked tasks
    Q_INVOKABLE void develop();

    /// Save current stack config to select file
    Q_INVOKABLE void saveToFile( QString path );

    /// Load current stack config from select file
    Q_INVOKABLE void loadFromFile( QString path );

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

signals:

    void developingChanged(bool arg);
    void progressChanged(double progress);
    void configChanged(QString config);

private slots:

    void onTaskStarted();
    void onTaskProgress(double progress);
    void onTaskFinished();

private:

    void setDeveloping(bool developing);
    void setProgress(double progress);
    void setConfig( QString config );
    CommonTasks *tasks() { return m_commonTasks; }

private:

    RoleMap           m_rolemap;
    QList<TaskBase*>  m_tasks;
    CommonTasks      *m_commonTasks;
    bool              m_developing;
    double            m_progress;
    QString           m_config;
};

#endif // TASKSTACK_H
