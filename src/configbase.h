#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QObject>
#include <QJsonObject>

class ConfigBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool canDisable READ canDisable NOTIFY canDisableChanged)
    Q_PROPERTY(bool dirty READ dirty NOTIFY dirtyChanged)

public:
    explicit ConfigBase(QString name, QObject *parent = 0);
    virtual ~ConfigBase();

    QString name() const { return m_name; }
    bool    enabled() const { return m_enabled; }
    bool    canDisable() const { return m_canDisable; }
    bool    dirty() const { return m_changes!=0; }
    size_t  changes() const { return m_changes; }

    virtual QJsonObject toJson() const;
    virtual bool fromJson( QJsonObject const &json );

public slots:

    void setEnabled(bool enabled);
    void setCanDisable(bool canDisable);

signals:

    void nameChanged(QString name);
    void enabledChanged(bool arg);
    void canDisableChanged(bool canDisable);
    void dirtyChanged(bool dirty);

protected slots:

    void markDirty();
    void resetDirty();

private:

    QString  m_name;
    bool     m_enabled;
    bool     m_canDisable;
    size_t   m_changes;
};

#endif // CONFIGBASE_H
