import QtQuick 2.0

TaskController {
    id: theController

    property real exposure: task!=undefined ? task.config.exposure : 0

    function setExposure(exposure) {
        if( task && task.config.exposure !== exposure ) {
            task.config.exposure = exposure
            triggerDevelop()
        }
    }
}

