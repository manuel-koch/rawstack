import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: theTask
    height: childrenRect.height + 2*theTask.spacing

    property TaskController controller
    property alias name: theHeader.name
    property real spacing: 4
    default property alias children: theColumn.children

    Column {
        id: theColumn
        anchors {
            top:         parent.top
            left:        parent.left
            right:       parent.right
            topMargin:   theTask.spacing
            leftMargin:  theTask.spacing
            rightMargin: theTask.spacing
        }

        TaskHeader {
            id: theHeader
            width:           parent.width
            onToggleEnabled: controller.toggleEnabled()
        }

        Binding {
            target:   theHeader
            property: "enabled"
            value:    controller && controller.task ? controller.task.config.enabled : false
            when:     controller && controller.task
        }

        Binding {
            target:   theHeader
            property: "canDisable"
            value:    controller && controller.task ? controller.task.config.canDisable : false
            when:     controller && controller.task
        }
    }
}

