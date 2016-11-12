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
import QtQuick 2.5

Item {
    id: theLabel

    property int   orientation: Qt.Horizontal
    property bool  isActive: false
    property bool  isLast:   false
    property alias text:     theText.text
    property real  spacing:  0

    signal clicked()

    states: [
        State {
            name: "horiz"
            when: theLabel.orientation == Qt.Horizontal
            changes: [
                PropertyChanges {
                    target: theLabel
                    width:  theText.width  + 2*theLabel.spacing
                    height: theText.height + 2*theLabel.spacing
                },
                AnchorChanges  {
                    target: theDivider
                    anchors.right:          theLabel.right
                    anchors.verticalCenter: theLabel.verticalCenter
                },
                PropertyChanges {
                    target: theDivider
                    anchors.rightMargin:    -theDivider.width/2 - theLabel.spacing/2
                    width:                  1
                    height:                 theLabel.height * 0.75
                }
            ]
        },
        State {
            name: "vert"
            when: theLabel.orientation == Qt.Vertical
            changes: [
                PropertyChanges {
                    target: theLabel
                    width:    theText.height + 2*theLabel.spacing
                    height:   theText.width  + 2*theLabel.spacing
                },
                PropertyChanges {
                    target: theText
                    rotation: -90
                },
                AnchorChanges  {
                    target: theDivider
                    anchors.bottom:           theLabel.bottom
                    anchors.horizontalCenter: theLabel.horizontalCenter
                },
                PropertyChanges {
                    target: theDivider
                    anchors.bottomMargin:    -theDivider.height/2 - theLabel.spacing/2
                    height:                   1
                    width:                    theLabel.width * 0.75
                }
            ]
        }
    ]

    Text {
        id: theText
        anchors.centerIn: parent
        font.bold:        isActive
    }

    Rectangle {
        id: theDivider
        color:   theText.color
        visible: isLast
    }

    MouseArea {
        anchors.fill: parent
        onClicked:    theLabel.clicked()
    }
}
