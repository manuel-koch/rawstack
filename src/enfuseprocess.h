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
#ifndef ENFUSEPROCESS_H
#define ENFUSEPROCESS_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QMap>
#include <QTemporaryFile>

class EnfuseProcess : public QProcess
{
    Q_OBJECT

public:
    explicit EnfuseProcess(QObject *parent = NULL);
    ~EnfuseProcess();

    QString output() { return m_output.fileName(); }
    QString &console() { return m_console; }

    void setInputs(const QStringList &inputs) { m_inputs = inputs; }
    void setExposureSigma(double sigma) { m_exposuresigma = sigma; }
    void run();

signals:

    void progress(double progress);

private slots:

    void onTerminated(int exitCode);
    void onStarted();
    void onStateChanged(QProcess::ProcessState newState);
    void onError(QProcess::ProcessError error);
    void onConsole();

private:

    int getNofLoaded();
    int getTotalLevels();
    int getNofLevels();

private:

    QTemporaryFile m_output;
    QString        m_console;
    QStringList    m_inputs;
    double         m_exposuresigma;
};

#endif // ENFUSEPROCESS_H
