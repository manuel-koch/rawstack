#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H

#include <QObject>
#include <QVariant>

class ConfigSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  task  READ task  CONSTANT)
    Q_PROPERTY(QString  group READ group CONSTANT)
    Q_PROPERTY(QString  name  READ name  CONSTANT)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:

    explicit ConfigSetting(QString fullname, QObject *parent = 0);

    QString fullname() const { return m_fullname; }
    QString task() const;
    QString group() const;
    QString name() const;
    QVariant value() const { return m_value; }

    /// Initialize value of setting with given data if it is currently null/invalid
    template <typename T>
    void initIfNull( const T &value )
    {
        QVariant v(value);
        if( m_value.isNull() || !m_value.canConvert(v.type()) || !m_value.convert(v.type()))
            m_value = v;
    }

signals:

    void valueChanged(QVariant value);

public slots:

    void setValue(QVariant value);

private:

    QString  m_fullname; // Dotted name of setting ( incl. task, group and name )
    QVariant m_value;    // Value of the setting
};

#endif // CONFIGSETTING_H
