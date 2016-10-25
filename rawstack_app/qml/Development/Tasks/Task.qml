/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
            value:    controller && controller.task ? controller.task.enabled : false
            when:     controller && controller.task
        }

        Binding {
            target:   theHeader
            property: "canDisable"
            value:    controller && controller.task ? controller.task.canDisable : false
            when:     controller && controller.task
        }
    }
}

