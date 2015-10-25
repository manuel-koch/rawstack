import QtQuick 2.0
import QtQuick.Layouts 1.1

import "../ConfigDb" as ConfigDb

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: theGrid.count

    ColumnLayout {

        anchors.fill: parent

        ConfigDb.Grid {
            id: theGrid
            Layout.preferredWidth: parent.width
            Layout.fillHeight:     true
        }

        Text {
            Layout.leftMargin:     4
            Layout.rightMargin:    4
            Layout.preferredWidth: parent.width
            wrapMode:              Text.WordWrap
            text:                  theGrid.count+" images..."
        }
    }
}
