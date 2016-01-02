#ifndef TASKSTACK_H
#define TASKSTACK_H

#include <Magick++.h>

#include <QAbstractListModel>
#include <QUrl>
#include <QFileInfo>

class ConfigDbEntry;
class TaskBase;
class CommonTasks;

class TaskStack : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool           developing READ developing NOTIFY developingChanged)
    Q_PROPERTY(double         progress   READ progress   NOTIFY progressChanged)
    Q_PROPERTY(CommonTasks*   tasks      READ tasks      CONSTANT)
    Q_PROPERTY(ConfigDbEntry* config     READ config     NOTIFY configChanged)
    Q_PROPERTY(bool           dirty      READ dirty      NOTIFY dirtyChanged)

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

    explicit TaskStack( bool preview, QObject *parent = NULL );
    virtual ~TaskStack();

    void    setWorkerThread( QThread *workerThread ) { m_workerThread = workerThread; }

    void           addTask(TaskBase *task, int idx = -1 );
    void           removeTask(int idx);
    bool           preview() const { return m_preview; }
    bool           developing() const { return m_developing; }
    double         progress() const { return m_progress; }
    ConfigDbEntry *config() const { return m_config; }
    bool           dirty() const { return m_dirty; }
    Magick::Image  gmimage();
    void           releaseImages();

public slots:

    /// Start developing the image with stacked tasks
    Q_INVOKABLE void develop();

    /// Load current stack config from selected configuration .
    Q_INVOKABLE void setConfig( ConfigDbEntry *config );

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

signals:

    void developingChanged(bool arg);
    void progressChanged(double progress);
    void configChanged(ConfigDbEntry *config);
    void dirtyChanged(bool dirty);

private slots:

    void onTaskStarted();
    void onTaskProgress(double progress);
    void onTaskFinished();
    void onTaskDirty(bool dirty);
    void onConfigDestroyed();

private:

    void setDeveloping(bool developing);
    void setProgress(double progress);
    void setDirty( bool dirty );
    CommonTasks *tasks() { return m_commonTasks; }

    /// Clear list of tasks for current raw image
    void clearTasks();

    /// Apply default tasks for current raw image
    void createDefaultTasks();

    /// Returns true when one of the tasks is dirty
    bool anyTaskDirty();

private:

    RoleMap           m_rolemap;
    QThread          *m_workerThread;   // hosting task worker in given thread
    QList<TaskBase*>  m_tasks;          // list of all tasks in stack
    CommonTasks      *m_commonTasks;    // helper to access named common tasks
    bool              m_developing;     // whether stack is currently developing
    bool              m_dirty;          // whether stack is currently dirty, i.e. needs to develop
    double            m_progress;       // current develop progress of stack ( 0...1 )
    ConfigDbEntry    *m_config;         // tasks configured by given configuration
    bool              m_preview;        // whether developing is using preview quality
};

#endif // TASKSTACK_H
