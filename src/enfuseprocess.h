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
