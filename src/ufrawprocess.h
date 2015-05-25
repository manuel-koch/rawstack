#ifndef UFRAWPROCESS_H
#define UFRAWPROCESS_H

#include <QObject>
#include <QProcess>

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

    QByteArray &out() { return m_outData; }
    QByteArray &err() { return m_errData; }

    void   run();
    int    shrink() const { return m_shrink; }
    double exposure() const { return m_exposure; }
    Interpolate interpolate() const { return m_interpolate; }
    Restore restore() const { return m_restore; }
    Clip clip() const { return m_clip; }

signals:

    void progress();

public slots:

    void setRaw( QString raw );
    void setShrink( int shrink );
    void setExposure( double exposure );
    void setInterpolate( Interpolate interpolate );
    void setRestore( Restore restore );
    void setClip( Clip clip );

private slots:

    void onTerminated(int exitCode);
    void onStarted();
    void onStateChanged(QProcess::ProcessState newState);
    void onError(QProcess::ProcessError error);
    void onOutData();
    void onErrData();

private:

    QString     m_raw;
    QByteArray  m_outData;
    QByteArray  m_errData;
    int         m_shrink;
    double      m_exposure;
    Interpolate m_interpolate;
    Restore     m_restore;
    Clip        m_clip;
};

#endif // UFRAWPROCESS_H
