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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

RowLayout {

    property alias text:        theText.text
    property alias cellsPerRow: theGridSlider.value

    Layout.preferredWidth: parent.width

    Text {
        id: theText
        Layout.leftMargin:     4
        Layout.rightMargin:    4
        Layout.topMargin:      2
        Layout.bottomMargin:   2
        Layout.fillWidth:      true
        wrapMode:              Text.WordWrap
        font.pointSize:        8
    }

    Slider {
        id: theGridSlider
        Layout.alignment:      Qt.AlignVCenter
        Layout.minimumWidth:   50
        Layout.maximumWidth:   100
        Layout.preferredWidth: parent.width/6
        Layout.leftMargin:     4
        Layout.rightMargin:    4
        minimumValue:          1
        stepSize:              1
        value:                 4
        maximumValue:          9
        style: SliderStyle {
            groove: Rectangle {
                        width: parent.width
                        height: 5
                        color: "lightgray"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
            handle: Rectangle {
                        anchors.centerIn: parent
                        color: control.pressed ? "white" : "lightgray"
                        border.color: "gray"
                        border.width: 2
                        implicitWidth: 10
                        implicitHeight: 10
                        radius: 5
                    }
        }
    }
}

