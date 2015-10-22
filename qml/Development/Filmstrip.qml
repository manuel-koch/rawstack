import QtQuick 2.0
import QtQuick.Layouts 1.1

import "../ConfigDb" as ConfigDb

Rectangle {
    id: theFilmStrip
    width:  200
    height: 50

    ColumnLayout {

        anchors.fill: parent
        spacing:      0

        ConfigDb.HorizontalList {
            id: theListQueue
            Layout.preferredWidth: parent.width
            Layout.fillHeight:     true
        }

        Text {
            Layout.leftMargin:     4
            Layout.rightMargin:    4
            Layout.preferredWidth: parent.width
            wrapMode:              Text.WordWrap
            text:                  theListQueue.count + " images..."
            font.pointSize:        8
        }
    }
}

