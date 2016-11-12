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

Item {
    id: theHeader

    width:  100
    height: 30

    property alias name:      theName.text
    property bool enabled:    true
    property bool canDisable: true

    signal toggleEnabled()

    RowLayout {
        id: theRow
        anchors.fill: parent

        Text {
            id: theName
            text:             "???"
            font.underline:   true
            font.weight:      Font.Bold
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }

        Rectangle {
            Layout.preferredWidth:  15
            Layout.preferredHeight: 15
            border.color:           "black"
            border.width:           1
            radius:                 4
            visible:                theHeader.canDisable

            Text {
                anchors.centerIn: parent
                text:             "E"
                color:            theHeader.enabled ? "black" : "silver"
                font.weight:      Font.Bold
            }

            MouseArea {
                anchors.fill: parent
                onClicked:    if(theHeader.canDisable) theHeader.toggleEnabled()
                propagateComposedEvents: true
            }
        }
    }
}

