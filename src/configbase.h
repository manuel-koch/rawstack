#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QObject>
#include <QDomElement>

class ConfigBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool canDisable READ canDisable NOTIFY canDisableChanged)

public:

    explicit ConfigBase(QString name, QObject *parent = 0);
    virtual ~ConfigBase();

    QString name() const { return m_name; }
    bool    enabled() const { return m_enabled; }
    bool    canDisable() const { return m_canDisable; }

    // trigger calculation hash of this configuration
    void rehash();

    // calculate hash of this configuration incl given base hash
    virtual QByteArray hash( const QByteArray &baseHash = QByteArray() );

    virtual QDomNode toXML(QDomNode &node ) const;
    virtual bool fromXML(QDomNode const &node );

public slots:

    void setEnabled(bool enabled);
    void setCanDisable(bool canDisable);

signals:

    void nameChanged(QString name);
    void enabledChanged(bool arg);
    void canDisableChanged(bool canDisable);
    void hashChanged();

private:

    void setHash( const QByteArray &hash );

private:

    QString    m_name;
    bool       m_enabled;
    bool       m_canDisable;
    QByteArray m_hash;
};

#endif // CONFIGBASE_H
