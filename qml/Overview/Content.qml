import QtQuick 2.0

import "../ConfigDb" as ConfigDb

Item {

    property alias cellsPerRow: theGrid.cellsPerRow
    property alias imageCount:  theGrid.count

    ConfigDb.Grid {
        id: theGrid
        anchors.fill: parent
    }

}

