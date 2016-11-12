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
    title: qsTr("&Image Grid")

    MenuItem {
        id: theGridNextItem
        text:     qsTr("Toggle Grid")
        shortcut: "Ctrl+G"
        enabled:  globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theGridNextItem
        menuEntry:   globalMenuModel.screenGridToggle
    }

    ExclusiveGroup {
        id: theGridGroup
    }

    MenuItem {
        id: theGridFiveItem
        text:           qsTr("Five")
        checkable:      true
        exclusiveGroup: theGridGroup
        enabled:        globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theGridFiveItem
        menuEntry:   globalMenuModel.screenGridFive
    }

    MenuItem {
        id: theGridTenItem
        text:           qsTr("Ten")
        checkable:      true
        exclusiveGroup: theGridGroup
        enabled:        globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theGridTenItem
        menuEntry:   globalMenuModel.screenGridTen
    }

    MenuItem {
        id: theGridTwentyItem
        text:           qsTr("Twenty")
        checkable:      true
        exclusiveGroup: theGridGroup
        enabled:        globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theGridTwentyItem
        menuEntry:   globalMenuModel.screenGridTwenty
    }

    MenuItem {
        id: theGridThirdsItem
        text:           qsTr("Thirds")
        checkable:      true
        exclusiveGroup: theGridGroup
        enabled:        globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theGridThirdsItem
        menuEntry:   globalMenuModel.screenGridThirds
    }

    MenuItem {
        id: theGridPhiItem
        text:           qsTr("Phi")
        checkable:      true
        exclusiveGroup: theGridGroup
        enabled:        globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theGridPhiItem
        menuEntry:   globalMenuModel.screenGridPhi
    }
}
