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
#ifndef MENUENTRY_H
#define MENUENTRY_H

#include <QObject>

class MenuEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)
    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)

public:

    explicit MenuEntry(QObject *parent = nullptr);
    virtual ~MenuEntry();

    QString title() const { return m_title; }
    bool enabled() const { return m_enabled; }
    bool checkable() const { return m_checkable; }
    bool checked() const { return m_checked; }

signals:

    // The menu entry has been triggered
    void triggered();

    // Title of menu entry changed
    void titleChanged(QString title);

    // Enabled state of menu entry changed
    void enabledChanged(bool enabled);

    // Checkable state of menu entry changed
    void checkableChanged(bool checkable);

    // Checked state of menu entry changed
    void checkedChanged(bool checked);

public slots:

    void setTitle(QString title);
    void setEnabled(bool enabled);
    void setCheckable(bool checkable);
    void setChecked(bool checked);

private:

    QString    m_title;
    bool       m_enabled;
    bool       m_checkable;
    bool       m_checked;
};

#endif // MENUENTRY_H
