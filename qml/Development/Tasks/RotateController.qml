import QtQuick 2.0

TaskController {
    id: theController

    property real degree: task!=undefined ? task.config.degree : 0

    function setDegree(degree) {
        if( task && task.config.degree !== degree ) {
            task.config.degree = degree
            triggerDevelop()
        }
    }
}

