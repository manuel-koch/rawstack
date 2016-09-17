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
import QtQuick.Controls 1.3
import com.rawstack.types 1.0

Item {
    property MenuItem  menuItem
    property MenuEntry menuEntry

    Connections {
        target: menuItem
        onTriggered: menuEntry.triggered()
    }

    Connections {
        target: menuEntry
        onTitleChanged:     menuItem.text      = menuEntry.title
        onCheckableChanged: menuItem.checkable = menuEntry.checkable
        onCheckedChanged:   menuItem.checked   = menuEntry.checked
    }

    Binding {
        target: menuEntry
        property: "title"
        value:    menuItem.text
    }

    Binding {
        target: menuEntry
        property: "checked"
        value:    menuItem.checked
    }

    Binding {
        target: menuEntry
        property: "checkable"
        value:    menuItem.checkable
    }
}
