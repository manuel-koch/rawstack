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

TabContainerLabels {
    id: theLabels

    anchors.top:         parent.top
    anchors.left:        parent.left
    anchors.leftMargin:  theCol.spacing
    anchors.bottom:      parent.bottom
    width:               theCol.width

    Column {
        id: theCol
        anchors.top:    parent.top
        anchors.left:   parent.left
        anchors.bottom: parent.bottom
        width:          childrenRect.width + spacing
        spacing:        5

        Repeater {
            id: theRepeater
            model: theLabels.titles
            TabContainerLabel {
                orientation: theLabels.orientation
                text:        theLabels.titles.get(index).title
                isActive:    index == currentIndex
                isLast:      (index+1) < theLabels.titles.count
                spacing:     theCol.spacing
                onClicked:   theLabels.labelClicked(index)
            }
        }
    }
}
