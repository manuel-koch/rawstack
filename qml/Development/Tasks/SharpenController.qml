import QtQuick 2.0

TaskController {
    id: theController

    property real radius: task!=undefined ? task.radius.value : 0
    property real sigma:  task!=undefined ? task.sigma.value : 0

    function setRadius(radius) {
        task.radius.value = radius
    }

    function setSigma(sigma) {
        task.sigma.value = sigma
    }
}

