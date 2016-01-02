import QtQuick 2.0

import com.rawstack.types 1.0

Item {
    id: theEntry

    property ConfigDbEntry config

    Rectangle {
        id: theBorderBg
        anchors.fill: parent
        color:        "#a0a0a0"
    }

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
        text:            title + "\n" + exif
        property string title: config.title + ( config.instance > 1 ? "." + config.instance : "" )
        property string exif:  config.exif.model + "\n" +
                               config.exif.lens + "\n" +
                               config.exif.exposure + ", " +
                               config.exif.fnumber + ", " +
                               config.exif.focallength + ", " +
                               config.exif.iso
    }

    MouseArea {
        id: theMouse
        anchors.fill: parent
        hoverEnabled: true
    }
}

