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
#ifndef COMMONTASKS_H
#define COMMONTASKS_H

#include <QObject>
#include <QUrl>

class TaskBase;

class CommonTasks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TaskBase* ufraw READ ufraw NOTIFY ufrawChanged)
    Q_PROPERTY(TaskBase* final READ final NOTIFY finalChanged)

public:

    explicit CommonTasks(QObject *parent = 0);
    ~CommonTasks();

    TaskBase* ufraw() const { return m_ufraw; }
    TaskBase* final() const { return m_final; }

    void setUfraw(TaskBase *ufraw);
    void setFinal(TaskBase *final);

signals:

    void ufrawChanged(TaskBase* ufraw);
    void finalChanged(TaskBase* final);

private:

    TaskBase* m_ufraw;
    TaskBase* m_final;
};

#endif // COMMONTASK_H
