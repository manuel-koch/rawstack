import QtQuick 2.0

Rectangle {
    id: theBg

    property alias text: theText.text

    color: "black"
    width:  theText.implicitWidth+radius
    height: theText.implicitHeight+radius

    Text {
        id: theText
        color:            "silver"
        anchors.centerIn: parent
        font.pointSize:   8
    }
}
