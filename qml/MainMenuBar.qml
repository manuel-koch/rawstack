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

MenuBar {

    signal openFile()
    signal openDir()

    signal loadDb()
    signal saveDb()
    signal saveAsDb()

    signal showOverview()
    signal showDevelop()
    signal showExport()

    Menu {
        title: qsTr("&File")

        MenuItem {
            text:        qsTr("Add image...")
            shortcut:    "Ctrl+Shift+O"
            onTriggered: openFile()
        }

        MenuItem {
            text:        qsTr("Add directory...")
            shortcut:    "Ctrl+Alt+O"
            onTriggered: openDir()
        }

        MenuSeparator { }

        MenuItem {
            text:        qsTr("Load database...")
            shortcut:    "Ctrl+O"
            onTriggered: loadDb()
        }

        MenuItem {
            text:        qsTr("Save database...")
            shortcut:    "Ctrl+S"
            enabled:     globalConfigDb.path
            onTriggered: saveDb()
        }

        MenuItem {
            text:        qsTr("Save database as...")
            shortcut:    "Ctrl+Shift+S"
            onTriggered: saveAsDb()
        }

        MenuSeparator { }

        MenuItem {
            text:        qsTr("Quit")
            shortcut:    "Ctrl+Q"
            onTriggered: Qt.quit();
        }
    }

    Menu {
        title: qsTr("&View")

        MenuItem {
            text:        qsTr("Overview")
            shortcut:    "Ctrl+1"
            onTriggered: showOverview()
        }

        MenuItem {
            text:        qsTr("Development")
            shortcut:    "Ctrl+2"
            onTriggered: showDevelop()
        }
    }
}
