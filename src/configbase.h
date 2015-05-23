#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QObject>

class ConfigBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool dirty READ dirty NOTIFY dirtyChanged)

public:
    explicit ConfigBase(QObject *parent = 0);
    virtual ~ConfigBase();

    bool dirty() const { return m_dirty; }

signals:

    void dirtyChanged(bool dirty);

protected slots:

    void setDirty(bool dirty);

private:

    bool m_dirty;
};

#endif // CONFIGBASE_H
