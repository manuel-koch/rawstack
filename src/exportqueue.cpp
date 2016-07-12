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
#include "exportqueue.h"
#include "exportsetting.h"
#include "exporttemplate.h"
#include "exportimage.h"
#include "configdbentry.h"

#include <QDebug>

ExportQueue::ExportQueue(QObject *parent)
    : QAbstractListModel(parent)
    , m_exporting(false)
    , m_progress(0)
{
    m_rolemap = QAbstractListModel::roleNames();
    m_rolemap[ExportRole] = "export";

    m_workerThread.setObjectName("ExportQueue-WorkerThread");
    m_workerThread.start();
}

ExportQueue::~ExportQueue()
{
    clearQueue();
    m_workerThread.quit();
    m_workerThread.wait();
}

void ExportQueue::enqueue(ExportTemplate *tmpl, ConfigDbEntry *cfg)
{
    ExportSetting *setting = new ExportSetting( cfg );
    setting->setImgQuality( tmpl->imgQuality() );
    setting->setImgType( tmpl->imgType() );
    setting->setImgPath( tmpl->imgPath() );

    qDebug() << "ExportQueue::enqueue()" << setting;

    beginInsertRows( QModelIndex(), m_exports.size(), m_exports.size() );

    ExportImage *exportImage = new ExportImage( &m_workerThread, setting, this );

    m_exports.append( exportImage );
    connect( exportImage, SIGNAL(progressChanged(double)), this, SLOT(onExportProgress(double)) );
    connect( exportImage, SIGNAL(exported()),              this, SLOT(onExportDone()) );

    endInsertRows();

    qDebug() << "ExportQueue::enqueue" << m_exports.size();

    if( !m_exporting )
    {
        setExporting( true );
        exportImage->exportImage();
    }
}

void ExportQueue::onExportProgress(double progress)
{
    ExportImage *exportImage = qobject_cast<ExportImage*>( sender() );
    int idx = m_exports.indexOf( exportImage );
    if( idx == -1 )
        return;
    qDebug() << "ExportQueue::onExportProgress" << exportImage << idx;
    double p = (double)idx / m_exports.size() + progress / m_exports.size();
    setProgress( p );
}

void ExportQueue::onExportDone()
{
    ExportImage *exportImage = qobject_cast<ExportImage*>( sender() );
    int idx = m_exports.indexOf( exportImage );
    if( idx == -1 )
        return;
    const bool haveMore = idx+1 < m_exports.size();
    qDebug() << "ExportQueue::onExportDone()" << idx << haveMore;
    setExporting( haveMore );
    if( haveMore )
        m_exports[idx+1]->exportImage();
}

void ExportQueue::setProgress(double progress)
{
    if( m_progress == progress )
        return;
    m_progress = progress;
    qDebug() << "ExportQueue::setProgress()" << m_progress;
    emit progressChanged(m_progress);
}

void ExportQueue::setExporting(bool exporting)
{
    if( m_exporting == exporting )
        return;
    m_exporting = exporting;
    qDebug() << "ExportQueue::setExporting()" << m_exporting;
    emit exportingChanged(m_exporting);
}

void ExportQueue::clearQueue()
{
    qDebug() << "ExportQueue::clearQueue()";
    beginResetModel();
    while( m_exports.size() )
    {
        delete m_exports.takeLast();
    }
    endResetModel();
}

int ExportQueue::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() )
        return 0;
    qDebug() << "ExportQueue::rowCount()" << m_exports.size();
    return m_exports.size();
}

QVariant ExportQueue::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_exports.size() )
        return QVariant();

    qDebug() << "ExportQueue::data()" << index.row() << m_rolemap[role];
    switch( role )
    {
        case Qt::DisplayRole:
        {
            return m_exports[index.row()]->setting()->config()->config();
        }
        case ExportRole:
        {
            return QVariant::fromValue<QObject*>( m_exports[index.row()] );
        }
    }
    return QVariant();
}
