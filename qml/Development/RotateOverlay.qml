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
    width:  100
    height: 100

    property point startPt:     Qt.point(0,0)
    property point endPt:       Qt.point(0,0)
    property real  distance:    0
    property real  minDistance: 20
    property real  degree:      0

    onStartPtChanged: {
        endPt           = startPt
        distance        = 0
        degree          = 0
        internal.degree = 0
        theCanvas.requestPaint()
    }

    onEndPtChanged: {
        var dx = endPt.x - startPt.x
        var dy = endPt.y - startPt.y
        distance = Math.sqrt( dx*dx + dy*dy )
        internal.degree = -180/Math.PI * Math.atan( dy / dx )
        if( Math.abs(internal.degree) <= 45 )
            degree = internal.degree // angle to x-axis
        else if( internal.degree > 45 && internal.degree <= 90 )
            degree = -(90-internal.degree) // angle to y-axis
        else if( internal.degree < -45 && internal.degree >= -90 )
            degree = 90+internal.degree // angle to y-axis
        theCanvas.requestPaint()
    }

    QtObject {
        id: internal
        property real degree
    }

    Canvas {
        id: theCanvas
        anchors.fill: parent
        renderTarget: Canvas.Image
        onPaint: {
            var ctx = getContext("2d")
            var alpha = 0.5
            var d = endPt.x > startPt.x ? distance : -distance
            var r = -Math.PI/180*internal.degree
            ctx.clearRect( 0, 0, width, height )

            if( distance <= minDistance )
                return

            // white line between start and end point
            ctx.save()
            ctx.globalAlpha = alpha
            ctx.strokeStyle = "white"
            ctx.lineWidth   = 2
            ctx.beginPath()
            ctx.translate( startPt.x, startPt.y )
            ctx.rotate( r )
            ctx.moveTo( 0, -0.5 );
            ctx.lineTo( d, -0.5 );
            ctx.stroke()
            ctx.closePath()
            ctx.restore()

            // black line between start and end point
            ctx.save()
            ctx.globalAlpha = alpha
            ctx.strokeStyle = "black"
            ctx.lineWidth   = 2
            ctx.beginPath()
            ctx.translate( startPt.x, startPt.y )
            ctx.rotate( r )
            ctx.moveTo( 0, 0.5 );
            ctx.lineTo( d, 0.5 );
            ctx.stroke()
            ctx.closePath()
            ctx.restore()

            // degree text in mids of start and end point
            ctx.save()
            ctx.globalAlpha = alpha
            ctx.fillStyle   = "white"
            ctx.strokeStyle = "black"
            ctx.font = "14px courier"
            ctx.beginPath()
            ctx.translate( startPt.x, startPt.y )
            ctx.rotate( r )
            var degreeText = degree.toFixed(2)+"°"
            var textBorder = 2
            var textWidth  = ctx.measureText(degreeText).width
            var textOffset = endPt.x > startPt.x ? distance/2 : - distance/2
            var textX      = textOffset - textWidth/2;
            ctx.text( degreeText, textX, -2 )
            ctx.fillRect( textX-textBorder, 0, textWidth+2*textBorder, -14 )
            ctx.stroke()
            ctx.closePath()
            ctx.restore()
        }
    }
}

