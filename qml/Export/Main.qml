import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: theExportQueue.title

    ExportQueue {
        id: theExportQueue
        anchors.fill: parent
    }
}
