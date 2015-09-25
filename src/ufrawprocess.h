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

    typedef QMap<QString,QString> InfoMap;

    QString output() { return m_output.fileName(); }
    QString &console() { return m_console; }

    void   run( bool probe );
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

private slots:

    void onTerminated(int exitCode);
    void onStarted();
    void onStateChanged(QProcess::ProcessState newState);
    void onError(QProcess::ProcessError error);
    void onConsole();

private:

    void buildArgs(bool probe, QStringList &args);
    void loadProbeSettings();

private:

    QTemporaryFile m_output;
    QString        m_console;

    QString     m_raw;
    int         m_shrink;
    double      m_exposure;
    bool        m_colorSmoothing;
    Interpolate m_interpolate;
    Restore     m_restore;
    Clip        m_clip;
    int         m_wbTemperature;
    double      m_wbGreen;
    InfoMap     m_info;
};

#endif // UFRAWPROCESS_H
