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
#include "testapplicationbase.h"

static int *g_argc   = nullptr;
static char **g_argv = nullptr;

TestApplicationBase::TestApplicationBase()
    : m_app(nullptr)
{
    //EMPTY
}

TestApplicationBase::~TestApplicationBase()
{
    destroyApplication();
}

void TestApplicationBase::initArguments(int *argc, char **argv)
{
    g_argc = argc;
    g_argv = argv;
}

void TestApplicationBase::SetUp()
{
    //EMPTY
}

void TestApplicationBase::TearDown()
{
    //EMPTY
}

QGuiApplication& TestApplicationBase::createApplication()
{
    if( !m_app  )
    {
        m_app = new QGuiApplication(*g_argc,g_argv);
    }

    return *m_app;
}

void TestApplicationBase::destroyApplication()
{
    if( m_app )
    {
        delete m_app;
        m_app = nullptr;
    }
}

void TestApplicationBase::exec(int milliSeconds)
{
    if( milliSeconds == 0)
    {
        QGuiApplication::instance()->exec();
    }
    else
    {
        QTimer t;
        t.setTimerType( Qt::PreciseTimer );
        QObject::connect(&t, &QTimer::timeout, this, &TestApplicationBase::onExecTimeout );
        t.setInterval(milliSeconds);
        t.start();
        QGuiApplication::instance()->exec();
    }
}

void TestApplicationBase::onExecTimeout()
{
    QGuiApplication::instance()->exit();
}
