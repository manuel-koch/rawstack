import QtQuick 2.0
import com.rawstack.types 1.0

Rectangle {
    id: theStack
    width:  100
    height: 300
    color:  "#a0a0a0"
    radius: 4

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
                console.log("Create task delegate for",model.name)
                if( model.name === "ufraw" )
                    source = "Tasks/UfrawTask.qml"
                else if( model.name === "dcraw" )
                    source = "Tasks/DcrawTask.qml"
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
                when:     item
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
            text: "Unknown Task: "+parent.delegateModel.name
        }
    }

    Component {
        id: highlightDelegate
        Rectangle {
            color:  "#f0f0f0";
            radius: theStack.radius
        }
    }

    ListView {
        id: theList
        anchors.fill:            parent
        model:                   theDevTaskStack
        highlight:               highlightDelegate
        highlightMoveDuration:   0
        highlightResizeDuration: 0
        maximumFlickVelocity:    300
        boundsBehavior:          Flickable.StopAtBounds
        delegate:                taskDelegate
        spacing:                 3
        focus:                   true
        clip:                    true
        onCurrentIndexChanged: {
            if( currentIndex != -1 )
                activeTask = currentItem.delegateModel.task
            else
                activeTask = null
        }
    }
}

