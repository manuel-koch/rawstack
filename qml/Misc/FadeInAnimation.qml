import QtQuick 2.5

SequentialAnimation {
    id: seq

    property variant target
    property alias   duration: anim.duration

    PropertyAction {
        // switch to visible at start of transition
        target:     seq.target
        property:   "visible"
        value:      true
    }

    PropertyAnimation {
        id:         anim
        target:     seq.target
        properties: "opacity"
        duration:   500
    }
}
