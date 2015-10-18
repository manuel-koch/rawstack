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

    void onDeveloped(bool developing);
    void onConfigChanged(QString config);

private:

    TaskStack     *m_stack;
    ExportSetting *m_setting;
    bool           m_exporting;
};

#endif // EXPORTIMAGE_H
