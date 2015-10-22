import QtQuick 2.0
import QtQuick.Layouts 1.1

import com.rawstack.types 1.0
import "../ConfigDb" as ConfigDb

Rectangle {
    id: theFilmStrip
    width:  200
    height: 50

    signal configSelected(ConfigDbEntry config)

    ColumnLayout {

        anchors.fill: parent
        spacing:      0

        ConfigDb.HorizontalList {
            id: theListQueue
            Layout.preferredWidth: parent.width
            Layout.fillHeight:     true
            onConfigSelected:      theFilmStrip.configSelected(config)
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

