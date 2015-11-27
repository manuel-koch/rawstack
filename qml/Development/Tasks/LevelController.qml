import QtQuick 2.0

TaskController {
    id: theController

    property real blackpoint: task!=undefined ? task.blackpoint.value : 0
    property real midpoint:   task!=undefined ? task.midpoint.value : 0
    property real whitepoint: task!=undefined ? task.whitepoint.value : 0
    property real brightness: task!=undefined ? task.brightness.value : 0
    property real saturation: task!=undefined ? task.saturation.value : 0

    function setBlackpoint(b) {
        task.blackpoint.value = b
    }

    function setMidpoint(m) {
        task.midpoint.value = m
    }

    function setWhitepoint(w) {
        task.whitepoint.value = w
    }

    function setBrightness(b) {
        task.brightness.value = b
    }

    function setSaturation(s) {
        task.saturation.value = s
    }
}

