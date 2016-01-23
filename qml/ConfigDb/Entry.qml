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

import com.rawstack.types 1.0

Item {
    id: theEntry

    property ConfigDbEntry config

    Rectangle {
        id: theUsableArea
        anchors.fill:    parent
        anchors.margins: 2
        radius:          4
        color:           "silver"
    }

    Image {
        id: theThumbnail
        anchors.fill:    theUsableArea
        asynchronous:    true
        source:          config != undefined ? ((width>256 || height>256) && config.final ? config.final : config.thumbnail) : ""
        fillMode:        Image.PreserveAspectFit
    }

    Rectangle {
        id: theHoverBg
        anchors { top:    theUsableArea.top;
                  left:   theUsableArea.left;
                  right:  theUsableArea.right;
                  bottom: theTitle.bottom; }
        color:        "white"
        visible:      theMouse.containsMouse
        opacity:      0.8
        radius:       theUsableArea.radius
    }

    Text {
        id: theTitle
        anchors { top:         theUsableArea.top;
                  left:        theUsableArea.left;
                  leftMargin:  4
                  right:       theUsableArea.right;
                  rightMargin: 4; }
        wrapMode:        Text.WrapAtWordBoundaryOrAnywhere
        font.pointSize:  10
        text:            config != undefined ? (config.title + ( config.instance > 1 ? "." + config.instance : "" )) : ""
    }

    Flow {
        id: theExifFlow
        anchors { bottom: theUsableArea.bottom; left: theUsableArea.left; right: theUsableArea.right }
        spacing: theUsableArea.radius
        visible: theMouse.containsMouse
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.model : ""
        }
        ExifText {
            radius:  theUsableArea.radius
            text:    config != undefined ? config.exif.lens : ""
            visible: config != undefined ? config.exif.lens : false
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.shutterText : ""
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.apertureText : ""
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.isoText : ""
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.focallenText : ""
        }
    }

    MouseArea {
        id: theMouse
        anchors.fill: parent
        hoverEnabled: true
    }
}

