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
#include "configsetting.h"

#include <QDebug>

class TestConfigSetting : public TestApplicationBase
{
    //EMPTY
};

TEST_F( TestConfigSetting, init)
{
    QString value1("foo");
    QString value2("bar");
    ConfigSetting s("something");

    EXPECT_TRUE( s.value().isNull() );

    s.initIfNull( value1 );
    EXPECT_FALSE( s.value().isNull() );
    EXPECT_EQ( s.value(), value1 );

    s.initIfNull( "oof" );
    EXPECT_FALSE( s.value().isNull() );
    EXPECT_EQ( s.value(), value1 );

    s.setValue( value2 );
    EXPECT_FALSE( s.value().isNull() );
    EXPECT_EQ( s.value(), value2 );
}

TEST_F( TestConfigSetting, names)
{
    QString task("task");
    QString group("hello");
    QString name("world");
    ConfigSetting s1(task+"."+group+"."+name);
    EXPECT_EQ( s1.task(), task );
    EXPECT_EQ( s1.group(), group );
    EXPECT_EQ( s1.name(), name );

    QString longgroup("hello.other");
    ConfigSetting s2(task+"."+longgroup+"."+name);
    EXPECT_EQ( s2.task(), task );
    EXPECT_EQ( s2.group(), longgroup );
    EXPECT_EQ( s2.name(), name );
}
