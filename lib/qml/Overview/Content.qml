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
import QtQuick.Layouts 1.1

import "../ConfigDb" as ConfigDb
import "../Misc" as Misc

Item {

    width:  400
    height: 400

    property alias cellsPerRow:   theGrid.cellsPerRow
    property alias imageCount:    theGrid.count
    property alias selImageCount: theGrid.selCount

    RowLayout {

        anchors.fill: parent
        spacing: 4

        Misc.TabContainer {
            id: theTabs
            Layout.preferredWidth: theTabs.implicitWidth
            Layout.fillHeight:     true
            orientation:           Qt.Vertical

            ExportTab { }

            Misc.TabContainerContent {
                title: "Foo"
                Rectangle {
                    anchors.fill: parent
                    color:        "red"
                }
            }

            Misc.TabContainerContent {
                title: "Bar"
                Rectangle {
                    anchors.fill: parent
                    color:        "blue"
                }
            }
        }

        ConfigDb.Grid {
            id: theGrid
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }
    }
}

