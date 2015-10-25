import QtQuick 2.0

Item {
    id: theScroll

    property Flickable flickable

    QtObject {
        id: internal
        property real posX: flickable.contentX / (flickable.contentWidth  - flickable.width)
        property real posY: flickable.contentY / (flickable.contentHeight - flickable.height)
    }

    Rectangle {
        id: theHoriz
        anchors.left:         parent.left
        anchors.leftMargin:   (flickable.width-width) * internal.posX
        anchors.bottom:       parent.bottom
        anchors.bottomMargin: 4
        height:               4
        width:                flickable.width / 6
        visible:              flickable.contentWidth > flickable.width
        radius:               2
    }

    Rectangle {
        id: theVert
        anchors.top:         parent.top
        anchors.topMargin:   (flickable.height-height) * internal.posY
        anchors.right:       parent.right
        anchors.rightMargin: 4
        height:              flickable.height / 6
        width:               4
        visible:             flickable.contentHeight > flickable.height
        radius:              2
    }
}

