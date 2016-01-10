import QtQuick 2.0

import com.rawstack.types 1.0

Item {
    id: theEntry

    property ConfigDbEntry config

    Rectangle {
        id: theUsableArea
        anchors.fill:    parent
        anchors.margins: 2
        radius:          4
        color:           "silver"
    }

    Image {
        id: theThumbnail
        anchors.fill:    theUsableArea
        anchors.bottomMargin: Math.min( theUsableArea.height/2, theExifFlow.height+theUsableArea.radius )
        asynchronous:    true
        source:          config != undefined ? ((width>256 || height>256) && config.final ? config.final : config.thumbnail) : ""
        fillMode:        Image.PreserveAspectFit
    }

    Rectangle {
        id: theHoverBg
        anchors { top:    theUsableArea.top;
                  left:   theUsableArea.left;
                  right:  theUsableArea.right;
                  bottom: theTitle.bottom; }
        color:        "white"
        visible:      theMouse.containsMouse
        opacity:      0.8
        radius:       theUsableArea.radius
    }

    Text {
        id: theTitle
        anchors { top:         theUsableArea.top;
                  left:        theUsableArea.left;
                  leftMargin:  4
                  right:       theUsableArea.right;
                  rightMargin: 4; }
        wrapMode:        Text.WrapAtWordBoundaryOrAnywhere
        font.pointSize:  10
        text:            config != undefined ? (config.title + ( config.instance > 1 ? "." + config.instance : "" )) : ""
    }

    Flow {
        id: theExifFlow
        anchors { bottom: theUsableArea.bottom; left: theUsableArea.left; right: theUsableArea.right }
        spacing: theUsableArea.radius
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.model : ""
        }
        ExifText {
            radius:  theUsableArea.radius
            text:    config != undefined ? config.exif.lens : ""
            visible: config != undefined ? config.exif.lens : false
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.shutterText : ""
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.apertureText : ""
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.isoText : ""
        }
        ExifText {
            radius: theUsableArea.radius
            text:   config != undefined ? config.exif.focallenText : ""
        }
    }

    MouseArea {
        id: theMouse
        anchors.fill: parent
        hoverEnabled: true
    }
}

