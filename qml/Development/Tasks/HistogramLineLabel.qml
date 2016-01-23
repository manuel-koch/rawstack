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

Text {
    id: theLabel

    property HistogramLine line

    font.pointSize: 10
    color:          line.color

    states: [
        State {
            name: "left"
            AnchorChanges { target: theLabel; anchors.right: line.left }
            PropertyChanges { target: theLabel; anchors.rightMargin: 2 }
            when: line.x > theLabel.implicitWidth
        },
        State {
            name: "right"
            AnchorChanges { target: theLabel; anchors.left: line.right }
            PropertyChanges { target: theLabel; anchors.leftMargin: 2 }
            when: line.x <= theLabel.implicitWidth
        }
    ]
}

