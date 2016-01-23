/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
