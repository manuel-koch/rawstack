import QtQuick 2.0
import QtQuick.Layouts 1.1
import com.rawstack.types 1.0

Rectangle {
    id: theQueue
    width:  100
    height: 300
    color:  "#a0a0a0"
    radius: 4

    property string title: "Exporting ( "+(globalExportQueue.progress*100).toFixed(1)+" % done )..."

    Component {
        id: exportDelegate
        Loader {
            id: theLoader
            height: 60
            width:  parent.width
            property var delegateModel: model
            Item {
                anchors.fill:    parent
                anchors.margins: 2
                ColumnLayout {
                    anchors.fill: parent
                    Text {
                        verticalAlignment: Text.AlignVCenter
                        elide:             Text.ElideMiddle
                        text:              delegateModel.config
                    }
                    Text {
                        verticalAlignment: Text.AlignVCenter
                        elide:             Text.ElideMiddle
                        text:              delegateModel.image.setting.imgPath
                    }
                    Rectangle {
                        Layout.preferredHeight: 4
                        Layout.preferredWidth:  parent.width*delegateModel.image.progress
                        color:                  "#fbfab8"
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked:    theList.currentIndex = delegateModel.index
                }
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
        spacing:                 2
        focus:                   true
        clip:                    true
    }
}

