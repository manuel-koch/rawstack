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
    title: qsTr("&View")

    MenuMode { }

    MenuImageSettings { }

    MenuImageZoom { }

    MenuImageGrid { }

    MenuItem {
        id: theExposureItem
        text:      qsTr("Image Exposure")
        shortcut:  "Ctrl+E"
        checkable: true
        enabled:   globalMenuModel.screenDevelop.checked
    }

    MenuConnection {
        menuItem:    theExposureItem
        menuEntry:   globalMenuModel.screenExposure
    }
}
