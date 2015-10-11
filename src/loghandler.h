#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>
#include <QDebug>
#include <QtDebug>
#include <QFile>
#include <QFile>

class LogHandler : public QObject
{
    Q_OBJECT

public:

    explicit LogHandler(QObject *parent = 0);
    virtual ~LogHandler();

public slots:

    void start( const QString &path );
    void stop();

private:

    static void setInstance( LogHandler *instance ) { ms_instance = instance; }
    static void onLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    void onLogImpl(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:

    static LogHandler *ms_instance;
    QFile m_log;
};

#endif // LOGHANDLER_H
