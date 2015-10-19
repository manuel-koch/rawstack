import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: theExportQueue.title

    ColumnLayout {

        anchors.fill: parent

        Text {
            Layout.leftMargin:     4
            Layout.rightMargin:    4
            Layout.preferredWidth: parent.width
            wrapMode:              Text.WordWrap
            text:                  "Exporting "+theExportQueue.nofQueued+" images..."
        }

        ExportQueue {
            id: theExportQueue
            Layout.preferredWidth: parent.width
            Layout.fillHeight:     true
        }

    }
}
