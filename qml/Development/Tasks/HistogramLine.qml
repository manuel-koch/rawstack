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

