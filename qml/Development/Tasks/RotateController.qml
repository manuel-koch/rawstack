import QtQuick 2.0

TaskController {
    id: theController

    property real degree: task!=undefined ? task.degree.value : 0

    function setDegree(degree) {
        task.degree.value = degree
    }
}

