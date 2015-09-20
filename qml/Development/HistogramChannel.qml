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

