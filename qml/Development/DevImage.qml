import QtQuick 2.0
import QtQuick.Controls 1.3
import com.rawstack.types 1.0

Rectangle {
    id: theDevImage

    width:  100
    height: 100
    color:  "silver"

    property TaskBase activeTask

    //    property DevelopmentRotateController rotateController: DevelopmentRotateController { }

    QtObject {
        id: internal
        property bool editRotate: activeTask ? activeTask.name == "rotate" : false
        property string zoomName: "fit"
        property real zoomFactor: 0
        property real zoomIdx:    0
        property var zoomValues:  [ 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 2, 4 ]
        property real zoomMin:    theFlickable.width / theImage.sourceSize.width

        function zoomFit() {
            zoomFactor = 0
            zoomIdx    = 3
            zoomName   = "fit"
        }

        function zoomIn() {
            if( theImage.status!=Image.Ready )
                return
            if( zoomFactor && zoomIdx+1 < zoomValues.length )
                zoomIdx++;
            else if ( !zoomFactor ) {
                for( var i=zoomValues.length-1; i>0; i-- ) {
                    if( zoomValues[i] > zoomMin ) {
                        zoomIdx = i
                    }
                }
            }
            zoomFactor = zoomValues[zoomIdx]
            zoomName   = zoomFactor * 100 + "%"
        }

        function zoomOut() {
            if( theImage.status!=Image.Ready )
                return
            if( zoomFactor && zoomIdx > 0 ) {
                zoomIdx--
                zoomFactor = zoomValues[zoomIdx]
                zoomName   = zoomFactor * 100 + "%"
            }
            if( zoomFactor < zoomMin )
                zoomFit()
        }
    }

    Flickable {
        id: theFlickable
        anchors.centerIn: parent
        width:  parent.width  - parent.width%4
        height: parent.height - parent.height%4
        contentWidth:   internal.zoomFactor==0 ? width  : theImage.sourceSize.width  * internal.zoomFactor
        contentHeight:  internal.zoomFactor==0 ? height : theImage.sourceSize.height * internal.zoomFactor
        clip:           true
        boundsBehavior: Flickable.StopAtBounds
        maximumFlickVelocity: 300

        Image {
            id: theImage
            anchors.fill: parent
            fillMode:     Image.PreserveAspectFit
            source:       globalDevTaskStack.tasks.final.images.image
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
            source:       globalDevTaskStack.tasks.final.images.underExposed
            color:        "yellow"
            visible:      false
            onSourceChanged: console.log("underexposed",source)
        }

        ColoredImage {
            id: theOverImage
            anchors.fill: parent
            fillMode:     Image.PreserveAspectFit
            source:       globalDevTaskStack.tasks.final.images.overExposed
            color:        "red"
            visible:      false
            onSourceChanged: console.log("overexposed",source)
        }

//        ImageGrid {
//            id: theGrid
//            anchors.top:        parent.top
//            anchors.left:       parent.left
//            anchors.topMargin:  (parent.height-height)/2
//            anchors.leftMargin: (parent.width-width)/2
//            width:              theImage.paintedWidth
//            height:             theImage.paintedHeight
//        }

//        RotateOverlay {
//            id: theRotateOverlay
//            anchors.top:        parent.top
//            anchors.left:       parent.left
//            anchors.topMargin:  (parent.height-height)/2
//            anchors.leftMargin: (parent.width-width)/2
//            width:              theImage.paintedWidth
//            height:             theImage.paintedHeight
//            visible:            internal.editRotate
//        }
    }

//    Row {
//        anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: -theZoomBadge.radius }
//        spacing: theZoomBadge.radius

//        InfoBadge {
//            id: theZoomBadge
//            text: "Zoom: "+internal.zoomName
//        }

//        InfoBadge {
//            id: theGridBadge
//            text: "Grid: "+theGrid.gridType
//            visible: theGrid.gridType
//        }
//    }

    MouseArea {
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

    Rectangle {
        id: theProgress
        visible:                  globalDevTaskStack.developing || progressHideTimer.running
        width:                    parent.width*0.75
        height:                   15
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom:           parent.bottom
        anchors.bottomMargin:     10
        color:                    Qt.rgba(1,1,1,0.8)
        radius:                   height/3
        ProgressBar {
            anchors.fill:    parent
            anchors.margins: 3
            minimumValue:    0
            maximumValue:    1
            value:           globalDevTaskStack.progress
        }
        Timer {
            id: progressHideTimer
            repeat:   false
            interval: 1000
        }
    }

}

