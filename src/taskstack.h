#ifndef TASKSTACK_H
#define TASKSTACK_H

#include <Magick++.h>

#include <QAbstractListModel>
#include <QUrl>
#include <QFileInfo>

class TaskBase;
class CommonTasks;
class CommonConfig;

class TaskStack : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool         developing READ developing NOTIFY developingChanged)
    Q_PROPERTY(double       progress   READ progress   NOTIFY progressChanged)
    Q_PROPERTY(CommonTasks* tasks      READ tasks      CONSTANT)
    Q_PROPERTY(QString      config     READ config     NOTIFY configChanged)
    Q_PROPERTY(bool         dirty      READ dirty      NOTIFY dirtyChanged)
    Q_PROPERTY(QString      raw        READ raw        NOTIFY rawChanged)
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

    void    addTask(TaskBase *task, int idx = -1 );
    void    removeTask(int idx);
    bool    preview() const { return m_preview; }
    bool    developing() const { return m_developing; }
    double  progress() const { return m_progress; }
    QString config() const { return m_config; }
    QString raw() const;
    bool    dirty() const { return m_dirty; }
    Magick::Image gmimage();
    void releaseImages();

public slots:

    /// Start developing the image with stacked tasks
    Q_INVOKABLE void develop();

    /// Save current stack config to select file
    Q_INVOKABLE void saveToFile( QString path );

    /// Load current stack config from select file.
    /// Selected path can be a RAW image or a rawstack configuration.
    Q_INVOKABLE void loadFromFile( QUrl url );

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

signals:

    void developingChanged(bool arg);
    void progressChanged(double progress);
    void configChanged(QString config);
    void rawChanged(QString raw);
    void dirtyChanged(bool dirty);

private slots:

    void onTaskStarted();
    void onTaskProgress(double progress);
    void onTaskFinished();
    void onTaskDirty(bool dirty);

private:

    void setDeveloping(bool developing);
    void setProgress(double progress);
    void setConfig( QString config );
    void setDirty( bool dirty );
    CommonTasks *tasks() { return m_commonTasks; }

    /// Clear list of tasks for current raw image
    void clearTasks();

    /// Load tasks from given raw image or saved task configuration
    bool loadTasks( QFileInfo const &file );

    /// Apply default tasks for given raw image
    void applyDefaultTasks( QFileInfo const &file );

    /// Returns true when one of the tasks is dirty
    bool anyTaskDirty();

    /// Use new common config instance
    void setCommonConfig( CommonConfig *common );

private:

    RoleMap           m_rolemap;
    QList<TaskBase*>  m_tasks;
    CommonTasks      *m_commonTasks;
    CommonConfig     *m_commonConfig;
    bool              m_developing;
    bool              m_dirty;
    double            m_progress;
    QString           m_config;
    bool              m_preview; // whether developing is using preview quality
};

#endif // TASKSTACK_H
