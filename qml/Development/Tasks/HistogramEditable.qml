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

    QtObject {
        id: internal

        property real mid: internal.midFactor(controller.blackpoint,controller.midpoint,controller.whitepoint)

        function midFactor(b,m,w) {
            var m_ = m >= 1 ? (m-1) / 10 + 0.5 : m * 0.5
            var f = b + (w-b)*m_
            console.log("midFactor",b,m,w,m_,f)
            return f
        }

        function midFromFactor(m_) {
            m_ = (m_-controller.blackpoint) / (controller.whitepoint-controller.blackpoint)
            var m = m_ <= 0.5 ? m_ / 0.5 : 1 + (m_ - 0.5)*10;
            console.log("midFromFactor",m_,m)
            return m;
        }
    }

    HistogramLine {
        id: theBlackpoint
        width:      linewidth
        x:          controller.blackpoint*theHistogram.width - width/2
        dragMinX:   -theBlackpoint.width/2
        dragMaxX:   theMidpoint.x-5
        onNewValue: controller.setBlackpoint( value )
    }

    HistogramLine {
        id: theMidpoint
        width:      linewidth
        x:          internal.mid*theHistogram.width - width/2
        dragMinX:   theBlackpoint.x+5
        dragMaxX:   theWhitepoint.x-5
        onNewValue: controller.setMidpoint( internal.midFromFactor( value ) )
    }

    HistogramLine {
        id: theWhitepoint
        width:      linewidth
        x:          controller.whitepoint*theHistogram.width - width/2
        dragMinX:   theMidpoint.x+5
        dragMaxX:   theHistogram.width - theWhitepoint.width/2
        onNewValue: controller.setWhitepoint( (theWhitepoint.x+theWhitepoint.width/2) / theHistogram.width )
    }

//    HistogramLine {
//        id: theWhitepoint
//        x:              controller.whitepoint*theHistogram.width - width/2
//        width:          linewidth
//        Drag.active:    whiteDragArea.drag.active
//        Drag.hotSpot.x: width/2
//        MouseArea {
//            id: whiteDragArea
//            propagateComposedEvents: true
//            cursorShape:    Qt.SizeHorCursor
//            anchors.fill:   parent
//            drag.target:    parent
//            drag.axis:      Drag.XAxis
//            drag.minimumX:  theMidpoint.x+5
//            drag.maximumX:  theHistogram.width - theWhitepoint.width/2
//            drag.threshold: 0
//            drag.onActiveChanged: {
//                if( !drag.active )
//                    controller.setWhitepoint( (theWhitepoint.x+theWhitepoint.width/2) / theHistogram.width )
//            }
//        }
//    }
}

