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
#include "testhelpermacros.h"
#include "configsettings.h"
#include "configsetting.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomNode>

class TestConfigSettings : public TestApplicationBase
{
    //EMPTY
};

TEST_F( TestConfigSettings, init)
{
    ConfigSettings s;

    // add four settings
    ConfigSetting* aaaBar        = s.getSetting("aaa.bar");
    ConfigSetting* aaaFoo        = s.getSetting("bbb.foo");
    ConfigSetting* cccHelloWorld = s.getSetting("ccc.hello.world");
    ConfigSetting* cccHelloSpace = s.getSetting("ccc.hello.space");

    // try getting settings via invalid name / indices
    EXPECT_NULL( s.getSetting("") );
    EXPECT_NULL( s.getSetting(-1) );
    EXPECT_NULL( s.getSetting(10) );

    // get settings via index
    ConfigSetting* set0 = s.getSetting((int)0);
    ConfigSetting* set1 = s.getSetting(1);
    ConfigSetting* set2 = s.getSetting(2);
    ConfigSetting* set3 = s.getSetting(3);
    EXPECT_EQ( aaaBar,        set0 );
    EXPECT_EQ( aaaFoo,        set1 );
    EXPECT_EQ( cccHelloWorld, set2 );
    EXPECT_EQ( cccHelloSpace, set3 );

    // check that four settings exist
    ASSERT_NOT_NULL( aaaBar );
    ASSERT_NOT_NULL( aaaFoo );
    ASSERT_NOT_NULL( cccHelloWorld );
    ASSERT_NOT_NULL( cccHelloSpace );
    EXPECT_EQ( s.settings().size(), 4 );
    EXPECT_EQ( s.tasks().size(), 3 );
    EXPECT_EQ( s.settings("aaa").size(), 1 );
    EXPECT_EQ( s.settings("bbb").size(), 1 );
    EXPECT_EQ( s.settings("ccc").size(), 2 );
    EXPECT_EQ( s.settings("ccc.hello").size(), 2 );

    aaaBar->setValue("xxx");

    // get setting again, setting list size is const
    ConfigSetting* aaaBar2 = s.getSetting("aaa.bar");
    EXPECT_EQ( s.settings().size(), 4 );
    EXPECT_EQ( aaaBar, aaaBar2 );

    // remove a setting, setting list size decreases
    s.removeSetting("aaa.bar");
    EXPECT_EQ( s.settings(), (QStringList() << "bbb.foo" << "ccc.hello.world" << "ccc.hello.space") );

    // remove settings of named task, setting list size decreases
    s.removeSetting("ccc");
    EXPECT_EQ( s.settings(), (QStringList() << "bbb.foo") );

    // get setting again, setting list size increases, setting value has changed
    ConfigSetting* aaaBar3 = s.getSetting("aaa.bar");
    EXPECT_EQ( s.settings().size(), 2 );

    // remove all settings, list is empty
    s.removeAll();
    EXPECT_EQ( s.settings().size(), 0 );
}

TEST_F( TestConfigSettings, copy)
{
    // create four settings and fill values
    ConfigSettings s;
    ConfigSetting* aaaBar  = s.getSetting("aaa.bar");
    ConfigSetting* aaaFoo  = s.getSetting("bbb.foo");
    ConfigSetting* cccTick = s.getSetting("ccc.tick");
    ConfigSetting* cccTock = s.getSetting("ccc.tock");
    ASSERT_NOT_NULL(aaaBar);
    ASSERT_NOT_NULL(aaaFoo);
    ASSERT_NOT_NULL(cccTick);
    ASSERT_NOT_NULL(cccTock);
    aaaBar->setValue("Hello");
    aaaFoo->setValue(1234);
    cccTick->setValue(1.25);
    cccTock->setValue(true);

    // copy settings
    ConfigSettings s2;
    s2 = s;
    ConfigSetting* aaaBar2  = s2.getSetting("aaa.bar");
    ConfigSetting* aaaFoo2  = s2.getSetting("bbb.foo");
    ConfigSetting* cccTick2 = s2.getSetting("ccc.tick");
    ConfigSetting* cccTock2 = s2.getSetting("ccc.tock");
    ASSERT_NOT_NULL(aaaBar2);
    ASSERT_NOT_NULL(aaaFoo2);
    ASSERT_NOT_NULL(cccTick2);
    ASSERT_NOT_NULL(cccTock2);
    EXPECT_EQ( aaaBar->value(), aaaBar2->value() );
    EXPECT_EQ( aaaFoo->value(), aaaFoo2->value() );
    EXPECT_EQ( cccTick->value(), cccTick2->value() );
    EXPECT_EQ( cccTock->value(), cccTock2->value() );
}

TEST_F( TestConfigSettings, xml)
{
    // create three settings and fill values
    ConfigSettings s;
    ConfigSetting* aaaBar  = s.getSetting("aaa.bar");
    ConfigSetting* aaaFoo  = s.getSetting("bbb.foo");
    ConfigSetting* cccTick = s.getSetting("ccc.tick");
    ConfigSetting* cccTock = s.getSetting("ccc.tock");
    ASSERT_NOT_NULL(aaaBar);
    ASSERT_NOT_NULL(aaaFoo);
    ASSERT_NOT_NULL(cccTick);
    ASSERT_NOT_NULL(cccTock);
    aaaBar->setValue("Hello");
    aaaFoo->setValue(1234);
    cccTick->setValue(1.25);
    cccTock->setValue(true);

    // create XML string from settings
    QDomDocument doc;
    QDomElement  root = doc.createElement("ROOT");
    doc.appendChild( root );
    s.toXML( root );
    QString xml = doc.toString( 0 );
    EXPECT_FALSE( xml.isEmpty() );

    // create settings from XML string
    QDomDocument doc2;
    EXPECT_TRUE( doc2.setContent( xml ) );
    ConfigSettings s2;
    s2.fromXML( doc2.documentElement() );

    // check that same seetings exist
    ConfigSetting* aaaBar2  = s2.getSetting("aaa.bar");
    ConfigSetting* aaaFoo2  = s2.getSetting("bbb.foo");
    ConfigSetting* cccTick2 = s2.getSetting("ccc.tick");
    ConfigSetting* cccTock2 = s2.getSetting("ccc.tock");
    ASSERT_NOT_NULL(aaaBar2);
    ASSERT_NOT_NULL(aaaFoo2);
    ASSERT_NOT_NULL(cccTick2);
    ASSERT_NOT_NULL(cccTock2);

    // check that seeting values are compatible
    EXPECT_TRUE( aaaBar2->value().canConvert(QVariant::String) );
    EXPECT_TRUE( aaaFoo2->value().canConvert(QVariant::Int) );
    EXPECT_TRUE( cccTick2->value().canConvert(QVariant::Double) );
    EXPECT_TRUE( cccTock2->value().canConvert(QVariant::Bool) );

    // check that seeting values are equal
    EXPECT_EQ( aaaBar->value(), aaaBar2->value() );
    EXPECT_DOUBLE_EQ( aaaFoo->value().toDouble(), aaaFoo2->value().toDouble() );
    EXPECT_EQ( cccTick->value(), cccTick2->value() );
    EXPECT_EQ( cccTock->value(), cccTock2->value() );
}
