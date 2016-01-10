import QtQuick 2.5
import QtQuick.Controls 1.3

import com.rawstack.types 1.0
import "Tasks"
import "../Misc" as Misc

Rectangle {
    id: theDevImage

    width:  100
    height: 100
    color:  "silver"

    property TaskBase activeTask

    property RotateController rotateController: RotateController { task: activeTask }

    QtObject {
        id: internal
        property bool   editRotate: activeTask ? activeTask.name === "rotate" : false
        property string zoomName:   "fit"
        property real   zoomFactor: 0
        property real   zoomIdx:    -1
        property var    zoomValues: [ 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2, 2.25, 2.5, 3, 4, 5, 7.5, 10 ]
        property real   zoomMin:    Math.min( theFlickable.height / theImage.sourceSize.height, theFlickable.width / theImage.sourceSize.width )

        function zoomFit() {
            console.log("Zoom fit")
            zoomApply(-1)
        }

        function zoomIn() {
            console.log("Zoom in: min",zoomMin)
            if( theImage.status!=Image.Ready )
                return
            if( zoomFactor && zoomIdx+1 < zoomValues.length )
                zoomApply( zoomIdx+1 )
            else if ( !zoomFactor ) {
                var idx = -1;
                for( var i=zoomValues.length-1; i>=0; i-- ) {
                    if( zoomValues[i] > zoomMin ) {
                        idx = i;
                    }
                }
                zoomApply( idx )
            }
        }

        function zoomOut() {
            console.log("Zoom in: min",zoomMin)
            if( theImage.status!=Image.Ready )
                return
            if( zoomFactor && zoomIdx > 0 ) {
                zoomApply( zoomIdx-1 )
            }
            if( zoomFactor < zoomMin )
                zoomFit()
        }

        function zoomApply(idx) {
            console.log("Zoom apply:",idx)
            var w, h;
            var wasFit = zoomFactor==0
            if( idx >= 0 && idx < zoomValues.length ) {
                zoomIdx    = idx
                zoomFactor = zoomValues[zoomIdx]
                zoomName   = zoomFactor * 100 + "%"
            }
            else {
                zoomIdx    = -1
                zoomFactor = 0
                zoomName   = "fit"
            }
            if( zoomFactor ) {
                w = theImage.sourceSize.width  * zoomFactor;
                h = theImage.sourceSize.height * zoomFactor;
            } else {
                w = theFlickable.width;
                h = theFlickable.height;
            }
            var c
            if( wasFit )
                c = Qt.point(0,0) // resizeContent() creates strange contentX/Y when zooming in from fit !?
            else if( zoomName === "fit" )
                c = Qt.point(0,0) // resizeContent() creates strange contentX/Y when zooming out to fit !?
            else
                c = Qt.point(theFlickable.contentX+theFlickable.width/2,theFlickable.contentY+theFlickable.height/2)
            theFlickable.resizeContent( w, h, c )
            theFlickable.returnToBounds()
        }
    }

    Flickable {
        id: theFlickable
        anchors.centerIn:     parent
        width:                parent.width  - parent.width%4
        height:               parent.height - parent.height%4
        clip:                 true
        boundsBehavior:       Flickable.StopAtBounds
        maximumFlickVelocity: 300

        onWidthChanged:    internal.zoomApply(internal.zoomIdx)
        onHeightChanged:   internal.zoomApply(internal.zoomIdx)

        rebound: Transition {
                NumberAnimation {
                    properties:  "x,y"
                    duration:    0
                    easing.type: Easing.Linear
                }
            }

        Image {
            id: theImage
            anchors.fill: parent
            fillMode:     Image.PreserveAspectFit
            source:       globalDevTaskStack.config ? globalDevTaskStack.config.final : ""
            Keys.onPressed: {
                if( event.key === Qt.Key_G ) {
                    theGrid.toggleGrid()
                }
                else if( event.key === Qt.Key_E ) {
                    theExposureTimer.running = !theExposureTimer.running
                    if( !theExposureTimer.running ) {
                        theUnderImage.visible = false
                        theOverImage.visible  = false
                    }
                }
                else if( event.key === Qt.Key_Plus ) {
                    internal.zoomIn()
                }
                else if( event.key === Qt.Key_Minus ) {
                    internal.zoomOut()
                }
                else if( event.key === Qt.Key_NumberSign ) {
                    internal.zoomFit()
                }
            }
        }

        Timer {
            id: theExposureTimer
            repeat:   true
            interval: 500
            onTriggered: {
                theUnderImage.visible = !theUnderImage.visible
                theOverImage.visible  = !theOverImage.visible
            }
        }

        ColoredImage {
            id: theUnderImage
            anchors.fill: parent
            fillMode:     Image.PreserveAspectFit
            source:       globalDevTaskStack.tasks.final ? globalDevTaskStack.tasks.final.images.underExposed : ""
            color:        "yellow"
            visible:      false
        }

        ColoredImage {
            id: theOverImage
            anchors.fill: parent
            fillMode:     Image.PreserveAspectFit
            source:       globalDevTaskStack.tasks.final ? globalDevTaskStack.tasks.final.images.overExposed : ""
            color:        "red"
            visible:      false
        }

        ImageGrid {
            id: theGrid
            anchors.top:        parent.top
            anchors.left:       parent.left
            anchors.topMargin:  (parent.height-height)/2
            anchors.leftMargin: (parent.width-width)/2
            width:              theImage.paintedWidth
            height:             theImage.paintedHeight
        }

        RotateOverlay {
            id: theRotateOverlay
            anchors.top:        parent.top
            anchors.left:       parent.left
            anchors.topMargin:  (parent.height-height)/2
            anchors.leftMargin: (parent.width-width)/2
            width:              theImage.paintedWidth
            height:             theImage.paintedHeight
            visible:            internal.editRotate
        }
    }

    Misc.FlickableScrollIndicator {
        id: theScroll
        anchors.fill: parent
        flickable:    theFlickable
    }

    Row {
        anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: -theZoomBadge.radius }
        spacing: theZoomBadge.radius

        InfoBadge {
            id: theZoomBadge
            text: "Zoom: "+internal.zoomName
        }

        InfoBadge {
            id: theGridBadge
            text:    "Grid: "+theGrid.gridType
            visible: theGrid.gridType
        }

        InfoBadge {
            id: theRotateBadge
            text:    "Rotate..."
            visible: internal.editRotate
        }
    }

    MouseArea {
        id: theMouseArea
        anchors.fill: parent
        onPressed: {
            theImage.focus = true
            if( internal.editRotate ) {
                var pt = mapToItem( theRotateOverlay, mouse.x, mouse.y )
                theRotateOverlay.startPt = Qt.point( pt.x, pt.y )
            }
        }
        onPositionChanged: {
            if( pressed && internal.editRotate ) {
                var pt = mapToItem( theRotateOverlay, mouse.x, mouse.y )
                theRotateOverlay.endPt = Qt.point( pt.x, pt.y )
            }
        }
        onReleased: {
            if( internal.editRotate && theRotateOverlay.distance > 30 ) {
                rotateController.setDegree( rotateController.degree + theRotateOverlay.degree )
                theRotateOverlay.startPt = theRotateOverlay.endPt = Qt.point( 0, 0 )
            }
        }
    }

    PinchArea {
        anchors.fill: parent

        property real lastHandledScale: 1
        property real zoomInScaleFactor: 1.3
        property real zoomOutScaleFactor: 1 / zoomInScaleFactor

        onPinchUpdated: {
            var diffScaleFactor = pinch.scale / lastHandledScale;
            console.log(diffScaleFactor)
            if( diffScaleFactor > zoomInScaleFactor )
            {
                lastHandledScale = pinch.scale
                internal.zoomIn()
            }
            else if( diffScaleFactor < zoomOutScaleFactor )
            {
                lastHandledScale = pinch.scale
                internal.zoomOut()
            }
        }

        onPinchFinished: lastHandledScale = 1
    }

    Rectangle {
        id: theProgress
        visible:                  globalDevTaskStack.developing || progressHideTimer.running
        width:                    parent.width*0.50
        height:                   20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter:   parent.verticalCenter
        color:                    "white"
        radius:                   height/2
        opacity:                  0.5
        Rectangle {
            anchors.left:           parent.left
            anchors.leftMargin:     parent.radius/2
            anchors.verticalCenter: parent.verticalCenter
            height:                 parent.height/2
            width:                  (parent.width-parent.radius) * globalDevTaskStack.progress
            radius:                 height/2
            color:                  "yellow"
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:   parent.verticalCenter
            font.pointSize:           10
            text:                     "Developing..."
        }
        Timer {
            id: progressHideTimer
            repeat:   false
            interval: 1000
        }
    }

}

