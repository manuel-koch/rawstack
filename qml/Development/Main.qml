import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: theScreen
    width:  640
    height: 480
    color:  "silver"

    property string title: theTaskStack.title

    GridLayout {
        id: theGrid
        rows:          3
        columns:       2
        anchors.fill:  parent
        columnSpacing: 2
        rowSpacing:    2

        property real stackWidth: 300

        HistogramFinal {
            Layout.preferredWidth:  theGrid.stackWidth
            Layout.preferredHeight: theGrid.stackWidth/2
        }

        DevImage {
            activeTask:        theTaskStack.activeTask
            Layout.rowSpan:    2
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }

        TaskStack {
            id: theTaskStack
            Layout.fillHeight:     true
            Layout.preferredWidth: theGrid.stackWidth
        }

        Filmstrip {
            Layout.columnSpan:      2
            Layout.fillWidth:       true
            Layout.preferredHeight: theScreen.height/4
            Layout.minimumHeight:   50
            Layout.maximumHeight:   150
        }
    }
}
