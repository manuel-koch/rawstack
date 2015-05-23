import QtQuick 2.0
//import com.rawstack.types 1.0

Rectangle {
    id: theStack
    width:  100
    height: 300
    color:  "#a0a0a0"
    radius: 4

//    property ImageTask activeTask

//    Component {
//        id: taskDelegate
//        Loader {
//            id: theLoader
//            height: item.height
//            width:  parent.width
//            property var myModel: model
//            Component.onCompleted: {
//                if( model.name === "ufraw" )
//                    source = "DevelopmentUfraw.qml"
//                else if( model.name === "dcraw" )
//                    source = "DevelopmentDcraw.qml"
//                else if( model.name === "rotate" )
//                    source = "DevelopmentRotate.qml"
//                else if( model.name === "level" )
//                    source = "DevelopmentLevel.qml"
//                else if( model.name === "sharpen" )
//                    source = "DevelopmentSharpen.qml"
//                else
//                    sourceComponent = defaultDelegate
//            }
//            Binding {
//                target:   item.controller
//                property: "task"
//                value:    theList.model.getTask(myModel.index)
//                when:     item
//            }
//            MouseArea {
//                anchors.fill: parent
//                onClicked:    theList.currentIndex = myModel.index
//            }
//        }
//    }

//    Component {
//        id: defaultDelegate
//        Text {
//            text: "Unknown Task: "+parent.myModel.name
//        }
//    }

//    Component {
//        id: highlightDelegate
//        Rectangle {
//            color:  "#f0f0f0";
//            radius: theStack.radius
//        }
//    }

//    ListView {
//        id: theList
//        anchors.fill:            parent
//        model:                   theDevCtrl.tasks
//        highlight:               highlightDelegate
//        highlightMoveDuration:   0
//        highlightResizeDuration: 0
//        maximumFlickVelocity:    300
//        boundsBehavior:          Flickable.StopAtBounds
//        delegate:                taskDelegate
//        spacing:                 3
//        focus:                   true
//        clip:                    true
//        onCurrentIndexChanged: {
//            if( currentIndex != -1 )
//                activeTask = theList.model.getTask(currentIndex)
//            else
//                activeTask = null
//        }
//    }
}

