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
#ifndef UFRAWPROCESS_H
#define UFRAWPROCESS_H

#include <QObject>
#include <QProcess>
#include <QTemporaryFile>
#include <QMap>

class UfrawProcess : public QProcess
{
    Q_OBJECT

public:
    explicit UfrawProcess(QObject *parent = NULL);
    ~UfrawProcess();

    enum Interpolate {
        InterpolateBilinear,
        InterpolatePpg,
        InterpolateFourColor,
        InterpolateVng,
        InterpolateAhd
    };

    enum Clip {
        ClipDigital,
        ClipFilm
    };

    enum Restore {
        RestoreClip,
        RestoreLch,
        RestoreHsv
    };

    enum Output {
        OutputProbe,
        OutputImage,
        OutputThumbnail
    };

    typedef QMap<QString,QString> InfoMap;

    QString output() { return m_output.fileName(); }
    QString &console() { return m_console; }

    void   run(Output output );
    bool   failed() const { return m_failed; }

    int    shrink() const { return m_shrink; }
    double exposure() const { return m_exposure; }
    bool colorSmoothing() const { return m_colorSmoothing; }
    Interpolate interpolate() const { return m_interpolate; }
    Restore restore() const { return m_restore; }
    Clip clip() const { return m_clip; }
    int wbTemperature() { return m_wbTemperature; }
    double wbGreen() { return m_wbGreen; }

    InfoMap const &info() { return m_info; }

signals:

    void progress();

public slots:

    void setRaw( QString raw );
    void setShrink( int shrink );
    void setExposure( double exposure );
    void setColorSmoothing( bool smooth );
    void setInterpolate( Interpolate interpolate );
    void setRestore( Restore restore );
    void setClip( Clip clip );
    void setWbTemperature( int wbTemperature );
    void setWbGreen( double wbGreen );
    void setWaveletDenoiseThreshold( int threshold );

private slots:

    void onTerminated(int exitCode);
    void onStarted();
    void onStateChanged(QProcess::ProcessState newState);
    void onError(QProcess::ProcessError error);
    void onConsole();

private:

    void buildArgs(Output output, QStringList &args);
    void loadProbeSettings();

private:

    QTemporaryFile m_output;
    QString        m_console;
    bool           m_failed;

    QString     m_raw;
    int         m_shrink;
    double      m_exposure;
    bool        m_colorSmoothing;
    Interpolate m_interpolate;
    Restore     m_restore;
    Clip        m_clip;
    int         m_wbTemperature;
    double      m_wbGreen;
    int         m_waveletDenoiseThreshold;
    InfoMap     m_info;
};

#endif // UFRAWPROCESS_H
