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
import QtQuick 2.0
import QtQuick.Controls 1.4

import com.rawstack.types 1.0

Menu {
    title: qsTr("&Image Settings")

    MenuItem {
        id: theCopySettingsItem
        text:     qsTr("Copy")
        shortcut: "Ctrl+C"
        enabled:  globalMenuModel.imgSettingsCopy.enabled
    }

    MenuConnection {
        menuItem:    theCopySettingsItem
        menuEntry:   globalMenuModel.imgSettingsCopy
    }

    MenuItem {
        id: theApplySettingsItem
        text:     qsTr("Apply")
        shortcut: "Ctrl+V"
        enabled:  globalMenuModel.imgSettingsApply.enabled
    }

    MenuConnection {
        menuItem:    theApplySettingsItem
        menuEntry:   globalMenuModel.imgSettingsApply
    }
}
