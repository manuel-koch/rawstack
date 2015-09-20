#ifndef HISTFACTORY_H
#define HISTFACTORY_H

#include <QObject>
#include <QVariant>

class WorkerBase;

class HistFactory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList red   READ red   NOTIFY redChanged)
    Q_PROPERTY(QVariantList green READ green NOTIFY greenChanged)
    Q_PROPERTY(QVariantList blue  READ blue  NOTIFY blueChanged)
    Q_PROPERTY(QVariantList lum   READ lum   NOTIFY lumChanged)

public:

    explicit HistFactory(WorkerBase *worker, QObject *parent = 0);

    const QVariantList &red() const { return m_red; }
    const QVariantList &green() const { return m_green; }
    const QVariantList &blue() const { return m_blue; }
    const QVariantList &lum() const { return m_lum; }

public slots:

    void onCycleChanged(int cycle);

signals:

    void redChanged(QVariantList red);
    void greenChanged(QVariantList green);
    void blueChanged(QVariantList blue);
    void lumChanged(QVariantList lum);

private:

    void setRed( const QVariantList &red );
    void setGreen( const QVariantList &green );
    void setBlue( const QVariantList &blue );
    void setLum( const QVariantList &lum );

private:

    WorkerBase  *m_worker;
    int          m_nofBins;
    QVariantList m_red;
    QVariantList m_green;
    QVariantList m_blue;
    QVariantList m_lum;
};

#endif // HISTFACTORY_H
