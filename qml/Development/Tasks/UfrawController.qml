import QtQuick 2.0

TaskController {
    id: theController

    property real fuse:     task!=undefined ? task.config.fuse     : 0
    property real exposure: task!=undefined ? task.config.exposure : 0

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
}

