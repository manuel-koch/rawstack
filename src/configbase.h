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

    bool   dirty() const { return m_changes!=0; }
    size_t changes() const { return m_changes; }

signals:

    void dirtyChanged(bool dirty);

protected slots:

    void markDirty();
    void resetDirty();

private:

    size_t m_changes;
};

#endif // CONFIGBASE_H
