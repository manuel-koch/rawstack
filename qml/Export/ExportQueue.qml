import QtQuick 2.0
import QtQuick.Layouts 1.1
import com.rawstack.types 1.0

Rectangle {
    id: theQueue
    width:  100
    height: 300
    color:  "#a0a0a0"
    radius: 4

    property string title:     "Export" +
                               (theList.count ? " ( "+(globalExportQueue.progress*100).toFixed(1)+" % done )..." : "")
    property int    nofQueued: theList.count

    Component {
        id: exportDelegate
        Item {
            property var delegateModel: model
            width:  parent.width
            height: theExportText.height
            Rectangle {
                id: theExportProgress
                anchors { top:    parent.top;
                          bottom: parent.bottom;
                          left:   parent.left;   }
                width:  parent.width*delegateModel.export.progress
                radius: theQueue.radius
                color:  "#c7fbb8"
            }
            Text {
                id: theExportText
                anchors { left:        parent.left;
                          right:       parent.right;
                          leftMargin:  theQueue.radius;
                          rightMargin: theQueue.radius; }
                wrapMode:       Text.WrapAtWordBoundaryOrAnywhere
                font.pointSize: 10
                text:           "Exporting "+delegateModel.export.setting.config+" to "+delegateModel.export.setting.imgPath+"..."
            }
            MouseArea {
                anchors.fill: parent
                onClicked:    theList.currentIndex = delegateModel.index
            }
        }
    }

    Component {
        id: highlightDelegate
        Rectangle {
            border { width: 1; color: "yellow" }
            color:  "#c0c0c0";
            radius: theQueue.radius
        }
    }

    ListView {
        id: theList
        anchors.fill:            parent
        model:                   globalExportQueue
        highlight:               highlightDelegate
        highlightMoveDuration:   0
        highlightResizeDuration: 0
        maximumFlickVelocity:    300
        boundsBehavior:          Flickable.StopAtBounds
        delegate:                exportDelegate
        spacing:                 4
        focus:                   true
        clip:                    true
    }

    Connections {
        target: globalExportQueue
        onExportingChanged: {
            if( !globalExportQueue.exporting )
                globalExportQueue.clearQueue()
        }
    }
}

