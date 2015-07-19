#ifndef UFRAWPROCESS_H
#define UFRAWPROCESS_H

#include <QObject>
#include <QProcess>
#include <QFile>
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

    QByteArray &out() { return m_outData; }
    QByteArray &err() { return m_errData; }

    void   run( bool probe );
    int    shrink() const { return m_shrink; }
    double exposure() const { return m_exposure; }
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
    void onOutData();
    void onErrData();

private:

    void buildArgs(QString probePath, QStringList &args);
    void loadProbeSettings(QFile &file);

private:

    QString     m_raw;
    QByteArray  m_outData;
    QByteArray  m_errData;
    int         m_shrink;
    double      m_exposure;
    Interpolate m_interpolate;
    Restore     m_restore;
    Clip        m_clip;
    int         m_wbTemperature;
    double      m_wbGreen;
    InfoMap     m_info;
};

#endif // UFRAWPROCESS_H
