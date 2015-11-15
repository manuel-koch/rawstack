import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import QtQml 2.2

import com.rawstack.types 1.0
import "../Misc" as Misc

Rectangle {
    id: theGrid
    width:  100
    height: 300
    color:  "#a0a0a0"

    property int cellsPerRow: 4
    property alias count:     theGridView.count
    property int selCount:    theSelectionModel.selectedIndexes.length

    onCellsPerRowChanged: theGridView.returnToBounds()

    Component {
        id: configDelegate
        Item {
            id: theDelegate

            property var delegateIsSelected: false
            property var delegateModel:      model

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

            Image {
                id: theThumbnail
                anchors.fill:    parent
                anchors.margins: 2
                asynchronous:    true
                source:          (width>256 || height>256) && delegateModel.config.final ? delegateModel.config.final : delegateModel.config.thumbnail
                fillMode:        Image.PreserveAspectFit
            }

            Rectangle {
                id: theHoverBg
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

            Rectangle {
                id: theSelectionBg
                anchors.fill: parent
                color:        "yellow"
                opacity:      0.2
                visible:      delegateIsSelected
            }

            MouseArea {
                id: theMouse
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onPressed: {
                    theGridView.currentIndex = delegateModel.index
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
