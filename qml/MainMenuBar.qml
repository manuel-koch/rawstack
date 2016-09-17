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

MenuBar {

    signal openFile()
    signal openDir()

    signal loadDb()
    signal saveDb()
    signal saveAsDb()

    signal showOverview()
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

        Menu {
            title: qsTr("&Mode")

            ExclusiveGroup {
                id: theScreenGroup
            }

            MenuItem {
                id: theOverviewItem
                text:           qsTr("Overview")
                shortcut:       "Ctrl+1"
                checkable:      true
                checked:        true
                exclusiveGroup: theScreenGroup
            }

            MainMenuItemConnection {
                menuItem:    theOverviewItem
                menuEntry:   globalMenuModel.screenOverview
            }

            MenuItem {
                id: theDevelopItem
                text:           qsTr("Development")
                shortcut:       "Ctrl+2"
                checkable:      true
                exclusiveGroup: theScreenGroup
            }

            MainMenuItemConnection {
                menuItem:    theDevelopItem
                menuEntry:   globalMenuModel.screenDevelop
            }
        }

        Menu {
            title: qsTr("&Image Grid")

            MenuItem {
                id: theGridNextItem
                text:     qsTr("Toggle Grid")
                shortcut: "Ctrl+G"
                enabled:  globalMenuModel.screenDevelop.checked
            }

            MainMenuItemConnection {
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

            MainMenuItemConnection {
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

            MainMenuItemConnection {
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

            MainMenuItemConnection {
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

            MainMenuItemConnection {
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

            MainMenuItemConnection {
                menuItem:    theGridPhiItem
                menuEntry:   globalMenuModel.screenGridPhi
            }
        }

        Menu {
            title: qsTr("&Image Zoom")

            MenuItem {
                id: theZoomInItem
                text:     qsTr("In")
                shortcut: "Ctrl++"
                enabled:  globalMenuModel.screenDevelop.checked
            }

            MainMenuItemConnection {
                menuItem:    theZoomInItem
                menuEntry:   globalMenuModel.screenZoomIn
            }

            MenuItem {
                id: theZoomOutItem
                text:     qsTr("Out")
                shortcut: "Ctrl+-"
                enabled:  globalMenuModel.screenDevelop.checked
            }

            MainMenuItemConnection {
                menuItem:    theZoomOutItem
                menuEntry:   globalMenuModel.screenZoomOut
            }

            MenuItem {
                id: theZoomFitItem
                text:     qsTr("Fit")
                shortcut: "Ctrl+#"
                enabled:  globalMenuModel.screenDevelop.checked
            }

            MainMenuItemConnection {
                menuItem:    theZoomFitItem
                menuEntry:   globalMenuModel.screenZoomFit
            }
        }

        MenuItem {
            id: theExposureItem
            text:      qsTr("Image Exposure")
            shortcut:  "Ctrl+E"
            checkable: true
            enabled:   globalMenuModel.screenDevelop.checked
        }

        MainMenuItemConnection {
            menuItem:    theExposureItem
            menuEntry:   globalMenuModel.screenExposure
        }
    }
}
