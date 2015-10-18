#ifndef EXPORTER_H
#define EXPORTER_H

#include <QAbstractListModel>
#include <QThread>

class ExportImage;
class ExportSetting;

class ExportQueue : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(double progress  READ progress  NOTIFY progressChanged)
    Q_PROPERTY(bool   exporting READ exporting NOTIFY exportingChanged)

public:

    typedef QHash<int, QByteArray> RoleMap;

    enum Roles
    {
        ImageRole = Qt::UserRole+1,
        ConfigRole,
        ProgressRole
    };

    explicit ExportQueue( QObject *parent = NULL );
    virtual ~ExportQueue();

    double progress() const { return m_progress; }
    bool   exporting() const { return m_exporting; }

public slots:

    /// Add given configuration to export queue, queue takes ownership of the export configuration
    Q_INVOKABLE void enqueue(ExportSetting *setting );

    /// Remove all queued exports
    Q_INVOKABLE void clearQueue();

public:

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const { return m_rolemap; }

signals:

    void progressChanged(double progress);
    void exportingChanged(bool exporting);

private slots:

    void onExportProgress(double progress);
    void onExportDone();

private:

    void setProgress(double progress);
    void setExporting(bool exporting);

private:

    RoleMap             m_rolemap;
    QList<ExportImage*> m_exports;
    QThread             m_workerThread;
    bool                m_exporting;
    double              m_progress;
};

#endif // EXPORTER_H
