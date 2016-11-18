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
#ifndef TESTAPPLICATIONBASE_H_8F98C1A7_99EA_4F09_8FDD_26E54AF13D64
#define TESTAPPLICATIONBASE_H_8F98C1A7_99EA_4F09_8FDD_26E54AF13D64

#include <gmock/gmock.h>

#include <QObject>
#include <QTimer>
#include <QGuiApplication>

/// Base class for unit tests that require a QGuiApplication
class TestApplicationBase : public QObject, public testing::Test
{
    Q_OBJECT

public:    

    TestApplicationBase();
    virtual ~TestApplicationBase();

    static void initArguments( int *argc, char **argv );

protected:

    virtual void SetUp();
    virtual void TearDown();

    QGuiApplication& createApplication();
    void destroyApplication();

    virtual void exec(int milliSeconds );

private slots:

    virtual void onExecTimeout();

private:

    QGuiApplication* m_app;
};

#endif // TESTAPPLICATIONBASE_H_8F98C1A7_99EA_4F09_8FDD_26E54AF13D64
