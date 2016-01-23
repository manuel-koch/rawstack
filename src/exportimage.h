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
#ifndef EXPORTIMAGE_H
#define EXPORTIMAGE_H

#include <QObject>

class TaskStack;
class ExportSetting;

class ExportImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ExportSetting* setting   READ setting   CONSTANT)
    Q_PROPERTY(bool           exporting READ exporting NOTIFY exportingChanged)
    Q_PROPERTY(double         progress  READ progress  NOTIFY progressChanged)

public:

    explicit ExportImage(QThread *workerThread, ExportSetting *setting, QObject *parent = 0);
    virtual ~ExportImage();

    ExportSetting* setting() const { return m_setting; }
    bool           exporting() const { return m_exporting; }
    double         progress() const;

signals:

    void exportingChanged(bool exporting);
    void progressChanged(double progress);
    void exported();

public slots:

    void exportImage();

private slots:

    void setExporting(bool exporting);

    void onDeveloping(bool developing);

    bool saveImage();
    void applyExif();

private:

    TaskStack     *m_stack;
    ExportSetting *m_setting;
    bool           m_exporting;
};

#endif // EXPORTIMAGE_H
