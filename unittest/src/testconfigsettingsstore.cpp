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
 * Copyright 2017 Manuel Koch
 */
#include "testapplicationbase.h"
#include "testhelpermacros.h"
#include "imagefactoryregistry.h"
#include "configsettingsstore.h"
#include "configsettings.h"
#include "configsetting.h"

#include <QDebug>

class TestConfigSettingsStore : public TestApplicationBase
{
public:
    void addTestSettings(ConfigDbEntry &entry)
    {
        EXPECT_EQ(entry.settings()->settings().count(), 0);

        // add four settings
        ConfigSetting* aaaBar        = entry.settings()->getSetting("aaa.bar");
        ConfigSetting* bbbFoo        = entry.settings()->getSetting("bbb.foo");
        ConfigSetting* cccHelloWorld = entry.settings()->getSetting("ccc.hello.world");
        ConfigSetting* cccHelloSpace = entry.settings()->getSetting("ccc.hello.space");

        aaaBar->setValue("xxx");
        bbbFoo->setValue(0.3);
        cccHelloWorld->setValue(500);
        cccHelloSpace->setValue(true);

        EXPECT_EQ(entry.settings()->settings().count(), 4);
    }

    void checkAllTestSettings(ConfigDbEntry &entry)
    {
        EXPECT_EQ(entry.settings()->settings().count(), 4);

        // get four settings
        ConfigSetting* aaaBar        = entry.settings()->getSetting("aaa.bar");
        ConfigSetting* bbbFoo        = entry.settings()->getSetting("bbb.foo");
        ConfigSetting* cccHelloWorld = entry.settings()->getSetting("ccc.hello.world");
        ConfigSetting* cccHelloSpace = entry.settings()->getSetting("ccc.hello.space");

        EXPECT_EQ(aaaBar->value(), "xxx");
        EXPECT_EQ(bbbFoo->value(), 0.3);
        EXPECT_EQ(cccHelloWorld->value(), 500);
        EXPECT_EQ(cccHelloSpace->value(), true);
    }

    void unselectTestSettings(ConfigSettingsStore &store)
    {
        EXPECT_EQ(store.nofSelectedSettings(), 4);
        store.setData( store.index(0,0), false, ConfigSettingsStore::SelectedRole);
        EXPECT_EQ(store.nofSelectedSettings(), 3);
    }

    void checkSomeTestSettings(ConfigDbEntry &entry)
    {
        EXPECT_EQ(entry.settings()->settings().count(), 4);

        // get four settings
        ConfigSetting* aaaBar        = entry.settings()->getSetting("aaa.bar");
        ConfigSetting* bbbFoo        = entry.settings()->getSetting("bbb.foo");
        ConfigSetting* cccHelloWorld = entry.settings()->getSetting("ccc.hello.world");
        ConfigSetting* cccHelloSpace = entry.settings()->getSetting("ccc.hello.space");

        EXPECT_EQ(aaaBar->value(), "yyy");
        EXPECT_EQ(bbbFoo->value(), 0.3);
        EXPECT_EQ(cccHelloWorld->value(), 500);
        EXPECT_EQ(cccHelloSpace->value(), true);
    }

protected:

    virtual void SetUp()
    {
        ImageFactoryRegistry::setInstance(&m_registry);
    }

    virtual void TearDown()
    {
        ImageFactoryRegistry::setInstance(nullptr);
    }

private:

    ImageFactoryRegistry m_registry;
};

TEST_F( TestConfigSettingsStore, copyAndApply_All)
{
    ConfigDbEntry e;
    ConfigSettingsStore s;

    addTestSettings(e);

    s.copyFrom( &e );
    EXPECT_EQ(s.nofSelectedSettings(), 4);

    ConfigDbEntry e2;
    s.applyTo( &e2 );

    checkAllTestSettings( e2 );

    unselectTestSettings( s );
    e2.settings()->getSetting("aaa.bar")->setValue("yyy");
    s.applyTo( &e2 );

    checkSomeTestSettings( e2 );
}
