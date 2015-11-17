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

            Entry {
                anchors.fill: parent
                config:       delegateModel.config
            }

            MouseArea {
                id: theMouse
                anchors.fill:    parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onPressed: {
                    theListView.currentIndex = delegateModel.index
                    if( mouse.button & Qt.RightButton )
                        theContextMenu.popup()
                    else
                        theContextMenu.visible = false
                }
                onDoubleClicked: globalDevTaskStack.setConfig( delegateModel.config )
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
                MenuItem {
                    text: "Purge"
                    onTriggered: delegateModel.config.purge()
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
