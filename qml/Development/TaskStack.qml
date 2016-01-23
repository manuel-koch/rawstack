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
import com.rawstack.types 1.0

Rectangle {
    id: theStack
    width:  100
    height: 300
    color:  "#a0a0a0"
    radius: 4

    property string   title:     "Development " + (globalDevTaskStack.config ? globalDevTaskStack.config.config : "")
    property TaskBase activeTask

    onActiveTaskChanged: console.log("activeTask",activeTask)

    Component {
        id: taskDelegate
        Loader {
            id: theLoader
            height: item.height
            width:  parent.width
            property var delegateModel: model
            Component.onCompleted: {
                console.log("Create task delegate for",model,model.index,model.name)
                if( model.name === "ufraw" )
                    source = "Tasks/UfrawTask.qml"
                else if( model.name === "rotate" )
                    source = "Tasks/RotateTask.qml"
                else if( model.name === "level" )
                    source = "Tasks/LevelTask.qml"
                else if( model.name === "sharpen" )
                    source = "Tasks/SharpenTask.qml"
                else
                    sourceComponent = defaultDelegate
            }
            Binding {
                target:   item.controller
                property: "task"
                value:    delegateModel.task
                when:     item && item.controller
            }
            MouseArea {
                anchors.fill: parent
                onClicked:    theList.currentIndex = delegateModel.index
            }
        }
    }

    Component {
        id: defaultDelegate
        Text {
            text: "Unknown Task"
        }
    }

    Component {
        id: highlightDelegate
        Rectangle {
            border { width: 1; color: "yellow" }
            color:  "#c0c0c0";
            radius: theStack.radius
        }
    }

    ListView {
        id: theList
        anchors.fill:            parent
        model:                   globalDevTaskStack
        highlight:               highlightDelegate
        highlightMoveDuration:   0
        highlightResizeDuration: 0
        maximumFlickVelocity:    300
        boundsBehavior:          Flickable.StopAtBounds
        delegate:                taskDelegate
        spacing:                 2
        focus:                   true
        clip:                    true
        onCurrentIndexChanged: {
            if( currentIndex != -1 )
                activeTask = currentItem.delegateModel.task
            else
                activeTask = null
        }
        onCountChanged: console.log("nof tasks",count)
    }

    Connections
    {
        target:          globalDevTaskStack
        onConfigChanged: globalDevTaskStack.develop()
    }

    Component.onCompleted: globalDevTaskStack.develop()
}

