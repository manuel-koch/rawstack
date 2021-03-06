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

Rectangle {
    id: theLine
    radius:         width/2
    color:          "black"
    opacity:        0.5
    anchors.top:    parent.top
    anchors.bottom: parent.bottom
    Drag.active:    theDragArea.drag.active
    Drag.hotSpot.x: width/2

    property real dragMinX
    property real dragMaxX

    signal dragDone()

    MouseArea {
        id: theDragArea
        propagateComposedEvents: true
        cursorShape:    Qt.SizeHorCursor
        anchors.fill:   parent
        drag.target:    parent
        drag.axis:      Drag.XAxis
        drag.minimumX:  dragMinX
        drag.maximumX:  dragMaxX
        drag.threshold: 0
        drag.onActiveChanged: {
            if( !drag.active )
                dragDone()
        }
    }
}

