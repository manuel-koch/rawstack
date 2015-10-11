import QtQuick 2.0

TaskController {
    id: theController

    property real fuse:          task!=undefined ? task.config.fuse     : 0
    property real exposure:      task!=undefined ? task.config.exposure : 0
    property real wbTemperature: task!=undefined ? task.config.wbTemperature : 0
    property real wbGreen:       task!=undefined ? task.config.wbGreen : 0

    function setFuse(fuse) {
        if( task && task.config.fuse !== fuse ) {
            task.config.fuse = fuse
            triggerDevelop()
        }
    }

    function setExposure(exposure) {
        if( task && task.config.exposure !== exposure ) {
            task.config.exposure = exposure
            triggerDevelop()
        }
    }

    function setWbTemperature(wbTemperature) {
        if( task && task.config.wbTemperature !== wbTemperature ) {
            task.config.wbTemperature = wbTemperature
            triggerDevelop()
        }
    }

    function setWbGreen(wbGreen) {
        if( task && task.config.wbGreen !== wbGreen ) {
            task.config.wbGreen = wbGreen
            triggerDevelop()
        }
    }
}

