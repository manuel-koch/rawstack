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
    title: qsTr("&Image Zoom")

    MenuItem {
        id: theZoomInItem
        text:     qsTr("In")
        shortcut: "Ctrl++"
        enabled:  globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theZoomInItem
        menuEntry:   globalMenuModel.screenZoomIn
    }

    MenuItem {
        id: theZoomOutItem
        text:     qsTr("Out")
        shortcut: "Ctrl+-"
        enabled:  globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theZoomOutItem
        menuEntry:   globalMenuModel.screenZoomOut
    }

    MenuItem {
        id: theZoomFitItem
        text:     qsTr("Fit")
        shortcut: "Ctrl+#"
        enabled:  globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theZoomFitItem
        menuEntry:   globalMenuModel.screenZoomFit
    }
}
