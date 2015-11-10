#include "configsetting.h"

#include <QDebug>

ConfigSetting::ConfigSetting(QString fullname, QObject *parent)
    : QObject(parent)
    , m_fullname(fullname)
{
}

QString ConfigSetting::task() const
{
    QStringList parts = m_fullname.split(".");
    Q_ASSERT(parts.size()>1);
    if( parts.isEmpty() )
        return m_fullname;
    else
        return parts.front();
}

QString ConfigSetting::group() const
{
    QStringList parts = m_fullname.split(".");
    Q_ASSERT(parts.size()>=2);
    QString group;
    for( int i=1; i<parts.size()-1; i++ )
        group += (group.isEmpty() ? "" : "." ) + parts[i];
    return group;
}

QString ConfigSetting::name() const
{
    QStringList parts = m_fullname.split(".");
    Q_ASSERT(parts.size()>=2);
    return parts.back();
}

void ConfigSetting::setValue(QVariant value)
{
    if( m_value == value )
        return;

    m_value = value;
    qDebug() << "ConfigSetting::setValue()" << m_fullname << m_value;
    emit valueChanged(m_value);
}

