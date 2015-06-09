import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width:  300
    height: 300

    property alias source:   theImage.source
    property alias fillMode: theImage.fillMode
    property alias color:    theOverlay.color

    Image {
        id: theImage
        anchors.fill: parent
        smooth:       true
        visible:      false
    }

    ColorOverlay {
        id: theOverlay
        anchors.fill: parent
        source:       theImage
    }
}
