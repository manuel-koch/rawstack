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
        onValueChanged: console.log("theBlackLine",value)
    }

    HistogramLineLabel {
        id: theBlackLabel
        anchors { top: theBlackLine.top; topMargin: theBlackLine.height/3 }
        line: theBlackLine
        text: theBlackLine.value.toFixed(2)
    }

    HistogramLine {
        id: theMidLine
        width:      linewidth
        x:          theBlackLine.x + (theWhiteLine.x-theBlackLine.x) * controller.midpoint
        dragMinX:   theBlackLine.x+2
        dragMaxX:   theWhiteLine.x-2
        onDragDone: controller.setMidpoint( value )
        color:      "yellow"
        property real value: (theMidLine.x-theBlackLine.x)/(theWhiteLine.x-theBlackLine.x)
        onValueChanged: console.log("theMidLine",value)
    }

    HistogramLineLabel {
        id: theMidLabel
        anchors { top: theBlackLabel.bottom; topMargin: 2 }
        line: theMidLine
        text: theMidLine.value.toFixed(2)
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
        onValueChanged: console.log("theWhiteLine",value)
    }

    HistogramLineLabel {
        id: theWhiteLabel
        anchors { top: theMidLabel.bottom; topMargin: 2 }
        line: theWhiteLine
        text: theWhiteLine.value.toFixed(2)
    }
}

