import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

import com.rawstack.types 1.0
import "../Misc" as Misc

Rectangle {
    id: theList
    width:  100
    height: 300
    color:  "#a0a0a0"

    property alias count: theListView.count

    function selectConfig(config)
    {
        var idx = globalConfigDb.indexOfConfig(config)
        if( idx !== -1 )
            theListView.currentIndex = idx;
    }

    Component {
        id: configDelegate
        Item {
            property var delegateIsCurrent: ListView.isCurrentItem
            property var delegateModel:     model
            width:  parent.height
            height: parent.height
            Image {
                anchors.fill:    parent
                anchors.margins: 2
                asynchronous:    true
                source:          delegateModel.config.thumbnail
                fillMode:        Image.PreserveAspectFit
            }
            Rectangle {
                anchors.fill: theText
                color:        "white"
                visible:      theMouse.containsMouse
                opacity:      0.3
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
                    theListView.currentIndex = delegateModel.index
                    if( mouse.button & Qt.RightButton )
                        theContextMenu.popup()
                    else
                        theContextMenu.visible = false
                }
                onDoubleClicked: globalDevTaskStack.loadConfig( delegateModel.config )
            }
            Menu {
                id: theContextMenu
                MenuItem {
                    text: "Duplicate"
                    onTriggered: delegateModel.config.duplicate()
                }
                MenuItem {
                    text: "Remove"
                    onTriggered: delegateModel.config.remove()
                }
            }
        }
    }

    Component {
        id: highlightDelegate
        Rectangle {
            border { width: 1; color: "yellow" }
            color:  "#c0c0c0";
            radius: theList.radius
        }
    }

    ListView {
        id: theListView
        orientation:             Qt.Horizontal
        anchors.fill:            parent
        model:                   globalConfigDb
        highlight:               highlightDelegate
        highlightMoveDuration:   0
        highlightResizeDuration: 0
        maximumFlickVelocity:    300
        boundsBehavior:          Flickable.StopAtBounds
        delegate:                configDelegate
        spacing:                 4
        focus:                   true
        clip:                    true
        snapMode:                ListView.SnapToItem
    }

    Misc.FlickableScrollIndicator {
        id: theScroll
        anchors.fill: parent
        flickable:    theListView
    }
}
