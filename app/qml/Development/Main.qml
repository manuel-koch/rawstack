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

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: theTaskStack.title

    GridLayout {
        id: theGrid
        rows:          3
        columns:       2
        anchors.fill:  parent
        columnSpacing: 2
        rowSpacing:    2

        property real stackWidth: 300

        HistogramFinal {
            Layout.preferredWidth:  theGrid.stackWidth
            Layout.preferredHeight: theGrid.stackWidth/2
            task: globalDevTaskStack.tasks.final
            text: "Output Histogram"
        }

        DevImage {
            activeTask:        theTaskStack.activeTask
            Layout.rowSpan:    2
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }

        TaskStack {
            id: theTaskStack
            Layout.fillHeight:     true
            Layout.preferredWidth: theGrid.stackWidth
        }

        Filmstrip {
            Layout.columnSpan:      2
            Layout.fillWidth:       true
            Layout.preferredHeight: 128
        }
    }
}
