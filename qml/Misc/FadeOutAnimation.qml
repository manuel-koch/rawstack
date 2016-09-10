import QtQuick 2.5

SequentialAnimation {
    id: seq

    property variant target
    property alias   duration: anim.duration

    PropertyAnimation {
        id:         anim
        target:     seq.target
        properties: "opacity"
        duration:   500
    }

    PropertyAction {
        // switch to hidden at end of transition
        target:     seq.target
        property:   "visible"
        value:      false
    }
}
