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
#include "testloghandler.h"

class TestEventListenerImpl : public ::testing::EmptyTestEventListener
{
public:

    TestEventListenerImpl(LogHandler &handler)
        : ::testing::EmptyTestEventListener()
        , m_handler(handler)
    {
    }

    virtual void OnTestStart(const ::testing::TestInfo& test_info)
    {
        m_testCaseName = test_info.test_case_name();
        m_testName     = test_info.name();
        m_failed       = false;
        m_handler.logToFile( QString("[ RUN      ] %1.%2")
                                .arg(m_testCaseName)
                                .arg(m_testName)
                             );
    }

    virtual void OnTestPartResult( const ::testing::TestPartResult& test_part_result)
    {
        if( test_part_result.failed() )
            m_failed = true;

        m_handler.logToFile( QString("[          ] %1.%2 at %3:%4\n%5")
                             .arg(m_testCaseName)
                             .arg(m_testName)
                             .arg(test_part_result.file_name())
                             .arg(test_part_result.line_number())
                             .arg(test_part_result.summary())
                             );
    }

    virtual void OnTestEnd(const ::testing::TestInfo& test_info) {
        Q_UNUSED(test_info);
        QString verdict(m_failed ? "[  FAILED  ]" : "[       OK ]");
        m_handler.logToFile( QString("%1 %2.%3")
                            .arg(verdict)
                            .arg(m_testCaseName)
                            .arg(m_testName)
                         );
    }

private:

    LogHandler &m_handler;
    QString     m_testCaseName;
    QString     m_testName;
    bool        m_failed;
};

TestLogHandler::TestLogHandler()
{
    // EMPTY
}

TestLogHandler::~TestLogHandler()
{
    // EMPTY
}

testing::TestEventListener *TestLogHandler::createTestEventListener()
{
    return new TestEventListenerImpl(*this);
}
