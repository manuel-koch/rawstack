import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

import com.rawstack.types 1.0
import "../Misc" as Misc

Rectangle {
    id: theGrid
    width:  100
    height: 300
    color:  "#a0a0a0"

    signal configSelected(ConfigDbEntry config)
    signal removeConfig(ConfigDbEntry config)

    property int cellsPerRow: 4
    property alias count: theGridView.count

    Component {
        id: configDelegate
        Item {
            property var delegateIsCurrent: GridView.isCurrentItem
            property var delegateModel:     model
            width:  theGridView.cellWidth
            height: theGridView.cellHeight
            Image {
                anchors.fill:    parent
                anchors.margins: 2
                asynchronous:    true
                source:          delegateModel.config.thumbnail
                fillMode:        Image.PreserveAspectFit
            }
            Rectangle {
                anchors { left:   parent.left;
                          top:    parent.top
                          right:  parent.right;
                          bottom: theText.bottom; }
                color:    "white"
                visible:  theMouse.containsMouse
                opacity:  0.3
            }
            Text {
                id: theText
                anchors { left:        parent.left;
                          leftMargin:  4
                          right:       parent.right;
                          rightMargin: 4; }
                wrapMode:        Text.WrapAtWordBoundaryOrAnywhere
                font.pointSize:  10
                text:            delegateModel.config.title
            }
            MouseArea {
                id: theMouse
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onPressed: {
                    theGridView.currentIndex = delegateModel.index
                    if( mouse.button & Qt.RightButton )
                        theContextMenu.popup()
                    else
                        theContextMenu.visible = false
                }
                onDoubleClicked: theGrid.configSelected( delegateModel.config )
            }
        }
    }

    Menu {
        id: theContextMenu

        MenuItem {
            text: "Remove"
            onTriggered: globalConfigDb.remove( theGridView.currentIndex )
        }
    }

    Component {
        id: highlightDelegate
        Rectangle {
            border { width: 1; color: "yellow" }
            color:  "#c0c0c0";
            radius: theGrid.radius
        }
    }

    GridView {
        id: theGridView
        anchors.fill:            parent
        model:                   globalConfigDb
        highlight:               highlightDelegate
        highlightMoveDuration:   0
        maximumFlickVelocity:    300
        boundsBehavior:          Flickable.StopAtBounds
        delegate:                configDelegate
        cellWidth:               parent.width / theGrid.cellsPerRow
        cellHeight:              Math.min( cellWidth, theGridView.height )
        focus:                   true
        clip:                    true
        snapMode:                GridView.SnapToRow
        flow:                    GridView.FlowLeftToRight

        Keys.onPressed: {
            if( event.key == Qt.Key_Minus )
                theGrid.cellsPerRow = Math.min( 9, theGrid.cellsPerRow+1 )
            else if( event.key == Qt.Key_Plus )
                theGrid.cellsPerRow = Math.max( 1, theGrid.cellsPerRow-1 )
        }
    }

    Misc.FlickableScrollIndicator {
        id: theScroll
        anchors.fill: parent
        flickable:    theGridView
        opacity:      0.5
    }
}
