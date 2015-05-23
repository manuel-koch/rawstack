import QtQuick 2.0

Rectangle {
    id: theToggle

    property bool enabled: true

    signal toggleChannel(bool exclusive)

    width:  10
    height: 10
    border.width: 1
    border.color: "black"
    opacity: enabled ? 1.0 : 0.3

    MouseArea {
        anchors.fill: parent
        onClicked: toggleChannel( mouse.modifiers & Qt.ShiftModifier )
    }
}

