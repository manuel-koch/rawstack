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

import com.rawstack.types 1.0

Item {
    id: theHistogram
    width:  100
    height: 100

    default property alias content: container.children

    property alias redEnabled:   theRedToggle.enabled
    property alias greenEnabled: theGreenToggle.enabled
    property alias blueEnabled:  theBlueToggle.enabled
    property alias lumEnabled:   theLumToggle.enabled

    property alias text: theTitle.text
    property TaskBase task

    QtObject {
        id: internal
        property real toggleSize: 10
        property int  nofEnabled: 4
        property real opacity:    1.0/nofEnabled
        property TaskHistogram histogram: TaskHistogram { task: theHistogram.task }

        function setToggle(toggle,enabled)
        {
            if( toggle.enabled !== enabled ) {
                toggle.enabled = enabled
                nofEnabled += toggle.enabled ? 1 : -1
                console.log(nofEnabled)
            }
        }

        function toggleChannel(toggle,exclusive)
        {
            var allEnabled = theRedToggle.enabled && theGreenToggle.enabled && theBlueToggle.enabled && theLumToggle.enabled
            if( (exclusive && !allEnabled) || !exclusive )
                setToggle( toggle, !toggle.enabled )
            if( exclusive ) {
                if( toggle !== theRedToggle )
                    internal.setToggle(theRedToggle,!toggle.enabled)
                if( toggle !== theGreenToggle )
                    internal.setToggle(theGreenToggle,!toggle.enabled)
                if( toggle !== theBlueToggle )
                    internal.setToggle(theBlueToggle,!toggle.enabled)
                if( toggle !== theLumToggle )
                    internal.setToggle(theLumToggle,!toggle.enabled)
            }
        }
    }

    HistogramChannel {
        id: theLumChannel
        objectName:   "lum"
        anchors.fill: parent
        bins:         internal.histogram.lum
        visible:      theLumToggle.enabled
        lineColor:    "black"
        fillColor:    Qt.rgba(0.2,0.2,0.2,0.25)
    }

    HistogramChannel {
        id: theRedChannel
        objectName:   "red"
        anchors.fill: parent
        bins:         internal.histogram.red
        visible:      theRedToggle.enabled
        lineColor:    "red"
        fillColor:    Qt.rgba(1,0,0,0.25)
    }

    HistogramChannel {
        id: theGreenChannel
        objectName:   "green"
        anchors.fill: parent
        bins:         internal.histogram.green
        visible:      theGreenToggle.enabled
        lineColor:    "green"
        fillColor:    Qt.rgba(0,1,0,0.25)
    }

    HistogramChannel {
        id: theBlueChannel
        anchors.fill: parent
        objectName:   "blue"
        bins:         internal.histogram.blue
        visible:      theBlueToggle.enabled
        lineColor:    "blue"
        fillColor:    Qt.rgba(0,0,1,0.25)
    }

    Row {
        id: theToggles
        spacing: 5
        anchors { top: parent.top; right: parent.right; topMargin: spacing; rightMargin: spacing }
        HistogramChannelToggle {
            id: theRedToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  "red"
            onToggleChannel: internal.toggleChannel(theRedToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
        HistogramChannelToggle {
            id: theGreenToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  "green"
            onToggleChannel: internal.toggleChannel(theGreenToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
        HistogramChannelToggle {
            id: theBlueToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  "blue"
            onToggleChannel: internal.toggleChannel(theBlueToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
        HistogramChannelToggle {
            id: theLumToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  Qt.rgba(0.9,0.9,0.9)
            onToggleChannel: internal.toggleChannel(theLumToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
    }

    Item {
        id: container
        anchors.fill: parent
    }

    Text {
        id: theTitle
        anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: 4; }
        horizontalAlignment: Text.Center
        font.pointSize: 8
    }
}

