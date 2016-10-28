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

Canvas {
    id: theChannel
    width:  100
    height: 100

    property var   bins: []
    property int   lineWidth: 2
    property color lineColor: "black"
    property color fillColor: "white"

    onBinsChanged:      theCanvas.requestPaint()
    onLineWidthChanged: theCanvas.requestPaint()
    onLineColorChanged: theCanvas.requestPaint()
    onFillColorChanged: theCanvas.requestPaint()

    Canvas {
        id: theCanvas
        anchors.fill: parent
        renderTarget: Canvas.Image
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect( 0, 0, width, height )

            var x, y, y_, v;
            var n = theChannel.bins.length
            console.debug(theChannel.objectName,n)
            if( !n )
                return
            var h  = height - theChannel.lineWidth
            var w  = width / (n+1)
            var w2 = w / 2

            ctx.save()
            ctx.beginPath()
            ctx.strokeStyle = theChannel.lineColor
            ctx.fillStyle   = theChannel.fillColor
            ctx.lineWidth   = theChannel.lineWidth
            ctx.moveTo( 0, height )
            for( var i=0; i<n; i++ )
            {
                y_ = y
                v  = theChannel.bins[i]
                x = i * w + w2
                y = height - 1 - ( h * Math.min(1,v) )
                ctx.lineTo( x, y )
            }
            ctx.lineTo( width, height )
            ctx.stroke()
            ctx.lineTo( 0,     height )
            ctx.fill()
            ctx.restore()
        }
    }
}

