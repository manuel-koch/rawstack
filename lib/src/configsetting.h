/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#ifndef CONFIGSETTING_H_E448C00A_43DF_4571_94ED_0074C1A2C9C0
#define CONFIGSETTING_H_E448C00A_43DF_4571_94ED_0074C1A2C9C0

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

    /// @brief Construct a named setting
    /// @param fullname is name with multiple parts separated by a dot
    explicit ConfigSetting(QString fullname, QObject *parent = 0);

    /// @returns full name of setting
    QString fullname() const { return m_fullname; }

    /// @returns first part of fullname, i.e. the task name
    QString task() const;

    /// @returns middle part(s) of fullname, i.e. the group name
    QString group() const;

    /// @returns last part of fullname, i.e. the name
    QString name() const;

    /// @returns current value of setting
    QVariant value() const { return m_value; }

    /// @brief Initialize value of setting with given data if it is currently null/invalid
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

    /// @brief Set value of setting
    void setValue(QVariant value);

private:

    QString  m_fullname; // Dotted name of setting ( incl. task, group and name )
    QVariant m_value;    // Value of the setting
};

#endif // CONFIGSETTING_H_E448C00A_43DF_4571_94ED_0074C1A2C9C0
