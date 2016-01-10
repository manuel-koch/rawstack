import QtQuick 2.0
import QtQuick.Layouts 1.1

import "../ConfigDb" as ConfigDb

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: "Overview of "+theGrid.count+" images"

    ColumnLayout {

        anchors.fill: parent
        spacing     : 0

        ConfigDb.Grid {
            id: theGrid
            Layout.preferredWidth: parent.width
            Layout.fillHeight:     true
            onCellsPerRowChanged:  theFooter.cellsPerRow = cellsPerRow
        }

        Footer {
            id: theFooter
            text: theGrid.count+" images..."
            onCellsPerRowChanged: theGrid.cellsPerRow = cellsPerRow
        }
    }
}
