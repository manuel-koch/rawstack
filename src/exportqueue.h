/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
        ExportRole = Qt::UserRole+1,
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
