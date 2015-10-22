import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

import com.rawstack.types 1.0

Rectangle {
    id: theHList
    width:  100
    height: 300
    color:  "#a0a0a0"

    signal configSelected(ConfigDbEntry config)
    signal removeConfig(ConfigDbEntry config)

    property alias count: theList.count

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
                    theList.currentIndex = delegateModel.index
                    if( mouse.button & Qt.RightButton )
                        theContextMenu.popup()
                    else
                        theContextMenu.visible = false
                }
                onDoubleClicked: theHList.configSelected( delegateModel.config )
            }
        }
    }

    Menu {
        id: theContextMenu

        MenuItem {
            text: "Remove"
            onTriggered: globalConfigDb.remove( theList.currentIndex )
        }
    }

    Component {
        id: highlightDelegate
        Rectangle {
            border { width: 1; color: "yellow" }
            color:  "#c0c0c0";
            radius: theHList.radius
        }
    }

    ListView {
        id: theList
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

    Rectangle {
        id: theScroll

        property real pos: theList.contentX / (theList.contentWidth - theList.width)

        anchors.left:         parent.left
        anchors.leftMargin:   (theList.width-width) * pos
        anchors.bottom:       parent.bottom
        anchors.bottomMargin: 4
        height:               4
        width:                theList.width / 6
        visible:              theList.contentWidth > theList.width
        radius:               2
        opacity:              0.5
    }
}
