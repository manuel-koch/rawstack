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
#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QObject>
#include <QMap>

class QThread;
class TaskBuilderBase;
class TaskBase;

class TaskFactory : public QObject
{
    Q_OBJECT

public:
    explicit TaskFactory(QObject *parent = 0);
    ~TaskFactory();

    static TaskFactory *getInstance() { return ms_instance; }
    static void setInstance( TaskFactory *instance ) { ms_instance = instance; }

public slots:

    /// Add an instance that can create a task instance, factory takes ownership of builder.
    bool add(TaskBuilderBase *builder);

    /// Create task of given name, worker of task will be hosted in given thread or default thread.
    /// Caller takes ownership of task.
    TaskBase *create(QString name, QThread *workerThread = NULL);

private:

    static TaskFactory *ms_instance;

    QThread                         *m_workerThread;  /// default worker thread for tasks
    QMap<QString,TaskBuilderBase*>   m_taskBuilder;   /// map of registered task builders
};

#endif // TASKFACTORY_H
