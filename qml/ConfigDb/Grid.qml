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
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import QtQml 2.2

import com.rawstack.types 1.0
import "../Misc" as Misc

Rectangle {
    id: theGrid

    width:  400
    height: 400

    color:  "#a0a0a0"

    property int cellsPerRow: 4
    property alias count:     theGridView.count
    property int selCount:    theSelectionModel.selectedIndexes.length

    onCellsPerRowChanged: theGridView.returnToBounds()

    Component {
        id: configDelegate
        Item {
            id: theDelegate

            property bool delegateIsSelected: false
            property var delegateModel:       model

            width:  theGridView.cellWidth
            height: theGridView.cellHeight

            function toggleSelection(extendSelection,forceSelection) {
                var cmd = forceSelection ? ItemSelectionModel.Select : ItemSelectionModel.Toggle;
                if( !extendSelection && !delegateIsSelected )
                    cmd |= ItemSelectionModel.Clear
                theSelectionModel.select( globalConfigDb.index( delegateModel.index, 0 ), cmd )
            }

            Connections {
                target: theSelectionModel
                onSelectionChanged: {
                    var idx = globalConfigDb.index( delegateModel.index, 0 )
                    theDelegate.delegateIsSelected = theSelectionModel.isSelected( idx )
                }
            }

            Entry {
                anchors.fill: parent
                config:       delegateModel.config
            }

            Rectangle {
                id: theSelectionBg
                anchors.fill: parent
                color:        "yellow"
                opacity:      0.2
                visible:      delegateIsSelected
            }

            MouseArea {
                id: theMouse
                anchors.fill:    parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onPressed: {
                    theGridView.currentIndex = delegateModel.index
                    theGridView.focus = true;
                    var isContextAction = mouse.button & Qt.RightButton
                    var extendSelection = mouse.modifiers & Qt.ControlModifier
                    theDelegate.toggleSelection( extendSelection, isContextAction )
                    if( isContextAction )
                        theContextMenu.popup()
                    else
                        theContextMenu.visible = false
                }
                onDoubleClicked: globalDevTaskStack.setConfig( delegateModel.config )
            }
        }
    }

    Menu {
        id: theContextMenu

        MenuItem {
            text: "Export"
            onTriggered: {
//                var cfgs = theSelectionModel.selectedConfigs()
//                for( var idx=0; idx<cfgs.length; idx++ )
//                    exportConfig( cfgs[idx] )
            }
        }
        MenuItem {
            text: "Duplicate"
            onTriggered: {
                var cfgs = theSelectionModel.selectedConfigs()
                for( var idx=0; idx<cfgs.length; idx++ )
                    cfgs[idx].duplicate()
            }
        }
        MenuItem {
            text: "Remove"
            onTriggered: {
                var cfgs = theSelectionModel.selectedConfigs()
                for( var idx=0; idx<cfgs.length; idx++ )
                    cfgs[idx].remove()
            }
        }
        MenuItem {
            text: "Purge"
            onTriggered: {
                var cfgs = theSelectionModel.selectedConfigs()
                for( var idx=0; idx<cfgs.length; idx++ )
                    cfgs[idx].purge()
            }
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

    ItemSelectionModel {
        id: theSelectionModel
        model: globalConfigDb

        function selectedConfigs() {
            var cfgs = [];
            console.log("selectedConfigs",ConfigDb.ConfigRole)
            for( var idx = 0; idx < selectedIndexes.length; idx++ )
                cfgs.push( globalConfigDb.data( selectedIndexes[idx], ConfigDb.ConfigRole ) );
            console.log( cfgs )
            return cfgs;
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
        cacheBuffer:             cellHeight*2

        Keys.onPressed: {
            if( event.key == Qt.Key_Space ) {
                var idx = globalConfigDb.index( currentIndex, 0 )
                theSelectionModel.select( idx, ItemSelectionModel.Toggle )
            }
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
    }
}
