import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: "Overview of "+theContent.imageCount+" images"

    ColumnLayout {

        anchors.fill: parent
        spacing     : 0

        Content {
            id: theContent
            Layout.preferredWidth: parent.width
            Layout.fillHeight:     true
            onCellsPerRowChanged:  theFooter.cellsPerRow = cellsPerRow
        }

        Footer {
            id: theFooter
            text: theContent.imageCount+" images..."
            onCellsPerRowChanged: theContent.cellsPerRow = cellsPerRow
        }
    }
}
