import QtQuick 2.0

Rectangle {
    id: theBadge
    color:   "white"
    opacity: 0.5
    width:  theLabel.contentWidth+2*radius
    height: theLabel.contentHeight+radius
    radius: 6

    property alias text: theLabel.text

    Text {
        id: theLabel
        anchors {
            left:         parent.left
            right:        parent.right
            bottom:       parent.bottom
        }
        horizontalAlignment: Qt.AlignHCenter
    }
}

