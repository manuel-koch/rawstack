import QtQuick 2.0

Item {
    id: theGrid

    width:  300
    height: 200

    property int    gridXSteps:    10
    property int    gridYSteps:    10
    property real   gridThickness: 0.5
    property color  gridColor:     "red"
    property string gridType:      ""

    onGridXStepsChanged:    theCanvas.requestPaint()
    onGridYStepsChanged:    theCanvas.requestPaint()
    onGridThicknessChanged: theCanvas.requestPaint()
    onGridColorChanged:     theCanvas.requestPaint()
    onGridTypeChanged:      theCanvas.requestPaint()

    function toggleGrid() {
        if( gridType == "" )
        {
            gridType = "grid-5"
            gridXSteps = 5
            gridYSteps = 5
        }
        else if( gridType == "grid-5" )
        {
            gridType = "grid-10"
            gridXSteps = 10
            gridYSteps = 10
        }
        else if( gridType == "grid-10" )
        {
            gridType = "grid-20"
            gridXSteps = 20
            gridYSteps = 20
        }
        else if( gridType == "grid-20" )
            gridType = "thirds"
        else if( gridType == "thirds" )
            gridType = "phi"
        else if( gridType == "phi" )
            gridType = ""
    }

    Canvas {
        id: theCanvas
        anchors.fill: parent
        renderTarget: Canvas.Image
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect( 0, 0, width, height )
            ctx.save()
            ctx.strokeStyle = theGrid.gridColor
            ctx.lineWidth   = gridThickness
            ctx.beginPath()
            if( gridType.substring(0,4) === "grid" ) {
                for( var i=1; i<theGrid.gridYSteps; i++ )
                {
                    var y = height * i / theGrid.gridYSteps;
                    ctx.moveTo( 0, y )
                    ctx.lineTo( width, y );
                }
                for( var i=1; i<theGrid.gridXSteps; i++ )
                {
                    var x = width * i / theGrid.gridXSteps;
                    ctx.moveTo( x, 0 )
                    ctx.lineTo( x, height );
                }
            }
            else if( gridType === "phi" || gridType === "thirds" ) {
                var f = gridType === "phi" ? 2.618 : 3
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

