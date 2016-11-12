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

Item {
    id: theScroll

    property Flickable flickable

    opacity: 0.25

    QtObject {
        id: internal
        property real posX: flickable.contentX / (flickable.contentWidth  - flickable.width)
        property real posY: flickable.contentY / (flickable.contentHeight - flickable.height)
    }

    Rectangle {
        id: theHoriz
        anchors.left:         parent.left
        anchors.leftMargin:   (flickable.width-width) * internal.posX
        anchors.bottom:       parent.bottom
        anchors.bottomMargin: 4
        height:               4
        width:                Math.max( 10, flickable.width * flickable.width / flickable.contentWidth )
        visible:              flickable.contentWidth > flickable.width
        radius:               2
        color:                "yellow"
    }

    Rectangle {
        id: theVert
        anchors.top:         parent.top
        anchors.topMargin:   (flickable.height-height) * internal.posY
        anchors.right:       parent.right
        anchors.rightMargin: 4
        height:              Math.max( 10, flickable.height * flickable.height / flickable.contentHeight )
        width:               4
        visible:             flickable.contentHeight > flickable.height
        radius:              2
        color:               "yellow"
    }
}

