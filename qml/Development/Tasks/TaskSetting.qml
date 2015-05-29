import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

RowLayout {
    anchors { left: parent.left; right: parent.right }

    property alias label: theLabel.text

    Text {
        id: theLabel
        text: "???"
    }
}

