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

Item {
    id: theGrid

    width:  300
    height: 200

    readonly property int grid_none:   0
    readonly property int grid_five:   1
    readonly property int grid_ten:    2
    readonly property int grid_twenty: 3
    readonly property int grid_thirds: 4
    readonly property int grid_phi:    5

    property real   gridThickness: 0.5
    property color  gridColor:     "red"
    property int    gridType:      grid_none
    property string gridTypeName:  "None"

    QtObject {
        id: internal
        property var gridTypeNames: ["None","Five","Ten","Twenty","Thirds","Phi"]
    }

    function toggleGrid() {
        gridType = (gridType<grid_phi) ? gridType+1 : grid_none
    }

    onGridThicknessChanged: theCanvas.requestPaint()
    onGridColorChanged:     theCanvas.requestPaint()
    onGridTypeChanged: {
        theCanvas.requestPaint();
        globalMenuModel.screenGridFive.checked   = (gridType==grid_five)
        globalMenuModel.screenGridTen.checked    = (gridType==grid_ten)
        globalMenuModel.screenGridTwenty.checked = (gridType==grid_twenty)
        globalMenuModel.screenGridThirds.checked = (gridType==grid_thirds)
        globalMenuModel.screenGridPhi.checked    = (gridType==grid_phi)
        gridTypeName = (gridType > grid_none && gridType <= grid_phi) ? internal.gridTypeNames[gridType] : grid_none
    }

    Connections {
        target: globalMenuModel.screenGridToggle
        onTriggered: toggleGrid()
    }
    Connections {
        target: globalMenuModel.screenGridFive
        onCheckedChanged: {
            if( globalMenuModel.screenGridFive.checked )
                gridType = grid_five
        }
    }
    Connections {
        target: globalMenuModel.screenGridTen
        onCheckedChanged: {
            if( globalMenuModel.screenGridTen.checked )
                gridType = grid_ten
        }
    }
    Connections {
        target: globalMenuModel.screenGridTwenty
        onCheckedChanged: {
            if( globalMenuModel.screenGridTwenty.checked )
                gridType = grid_twenty
        }
    }
    Connections {
        target: globalMenuModel.screenGridThirds
        onCheckedChanged: {
            if( globalMenuModel.screenGridThirds.checked )
                gridType = grid_thirds
        }
    }
    Connections {
        target: globalMenuModel.screenGridPhi
        onCheckedChanged: {
            if( globalMenuModel.screenGridPhi.checked )
                gridType = grid_phi
        }
    }

    Canvas {
        id: theCanvas
        anchors.fill: parent
        renderTarget: Canvas.Image
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect( 0, 0, width, height )
            if( gridType == grid_none )
                return
            ctx.save()
            ctx.strokeStyle = theGrid.gridColor
            ctx.lineWidth   = gridThickness
            ctx.beginPath()
            if( gridType >= grid_five && gridType <= grid_twenty ) {
                var gridSteps
                if( gridType == grid_five )
                    gridSteps = 5
                else if( gridType == grid_ten )
                    gridSteps = 10
                else if( gridType == grid_twenty )
                    gridSteps = 20
                for( var i=1; i<gridSteps; i++ )
                {
                    var y = height * i / gridSteps;
                    var x = width * i / gridSteps;
                    ctx.moveTo( 0, y )
                    ctx.lineTo( width, y );
                    ctx.moveTo( x, 0 )
                    ctx.lineTo( x, height );
                }
            }
            else if( gridType == grid_thirds || gridType == grid_phi ) {
                var f = gridType == grid_phi ? 2.618 : 3
                var a = width / f
                var b = height / f
                var x1 = a
                var x2 = width-a
                var y1 = b
                var y2 = height-b;
                ctx.moveTo( x1, 0 );
                ctx.lineTo( x1, height );
                ctx.moveTo( x2, 0 );
                ctx.lineTo( x2, height );
                ctx.moveTo( 0,     y1 );
                ctx.lineTo( width, y1 );
                ctx.moveTo( 0,     y2 );
                ctx.lineTo( width, y2 );
            }
            ctx.stroke()
            ctx.closePath()
            ctx.restore()
        }
    }
}

