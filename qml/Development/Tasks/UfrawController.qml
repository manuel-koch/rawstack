import QtQuick 2.0

TaskController {
    id: theController

    property real fuse:          task!=undefined ? task.fuse.value          : 0
    property real exposuresigma: task!=undefined ? task.exposuresigma.value : 0
    property real exposure:      task!=undefined ? task.exposure.value      : 0
    property real wbTemperature: task!=undefined ? task.wbTemp.value        : 0
    property real wbGreen:       task!=undefined ? task.wbGreen.value       : 0

    function setFuse(fuse) {
        task.fuse.value = fuse
    }

    function setExposureSigma(sigma) {
        task.exposuresigma.value = sigma
    }

    function setExposure(exposure) {
        task.exposure.value = exposure
    }

    function setWbTemperature(wbTemperature) {
        task.wbTemp.value = wbTemperature
    }

    function setWbGreen(wbGreen) {
        task.wbGreen.value = wbGreen
    }
}

