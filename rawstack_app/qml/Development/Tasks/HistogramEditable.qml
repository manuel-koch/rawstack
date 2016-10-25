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

import ".." as Development

Development.HistogramFinal {
    id: theHistogram
    width:  100
    height: 100

    property LevelController controller
    property int linewidth: 1

    task: globalDevTaskStack.tasks.ufraw

    HistogramLine {
        id: theBlackLine
        width:      linewidth
        x:          controller.blackpoint*theHistogram.width - width/2
        dragMinX:   -theBlackLine.width/2
        dragMaxX:   theWhiteLine.x-2
        onDragDone: controller.setBlackpoint( value )
        color:      "black"
        property real value: (x+width/2) / theHistogram.width
    }

    HistogramLineLabel {
        id: theBlackLabel
        anchors { top: theBlackLine.verticalCenter; topMargin: theBlackLabel.implicitHeight }
        line: theBlackLine
        text: theBlackLine.value.toFixed(3)
    }

    HistogramLine {
        id: theMidLine
        width:      linewidth
        x:          -width/2 + m2f(controller.midpoint) * theHistogram.width
        dragMinX:   theBlackLine.dragMinX
        dragMaxX:   theWhiteLine.dragMaxX
        onDragDone: controller.setMidpoint( value )
        color:      "yellow"
        property real value: f2m( (x+width/2) / theHistogram.width )

        // map factor 0...1 to midpoint 0.1 ... 10
        function f2m( f ) {
            var m;
            if( f <= 0.5 )
                m = 0.1 + f * 2 * 0.9
            else
                m = 1.0 + (f-0.5) * 2 * 9
            return m;
        }

        // map midpoint 0.1 ... 10 to factor 0...1
        function m2f( m ) {
            var f;
            if( m <= 1.0 )
                f = (m-0.1) / 0.9 * 0.5
            else
                f = 0.5 + (m-1.0) / 9 / 2
            return f
        }
    }

    HistogramLineLabel {
        id: theMidLabel
        anchors { verticalCenter: theMidLine.verticalCenter }
        line: theMidLine
        text: theMidLine.value.toFixed(3)
    }

    HistogramLine {
        id: theWhiteLine
        width:      linewidth
        x:          controller.whitepoint*theHistogram.width - width/2
        dragMinX:   theBlackLine.x+2
        dragMaxX:   theHistogram.width - theWhiteLine.width/2
        onDragDone: controller.setWhitepoint( value )
        color:      "white"
        property real value: (x+width/2) / theHistogram.width
    }

    HistogramLineLabel {
        id: theWhiteLabel
        anchors { bottom: theWhiteLine.verticalCenter; bottomMargin: theWhiteLabel.implicitHeight }
        line: theWhiteLine
        text: theWhiteLine.value.toFixed(3)
    }
}

