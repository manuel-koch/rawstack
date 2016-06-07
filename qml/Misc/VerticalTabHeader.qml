import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: theHeader

    width:  childrenRect.width
    height: childrenRect.height

    property var   titles: ["red","green","blue"]
    property color selectedFontColor: "black"
    property color selectedBgColor:   Qt.lighter(Qt.rgba(0.9,0.9,0.9))
    property color otherFontColor:    Qt.lighter(selectedFontColor)
    property color otherBgColor:      "#a0a0a0"
    property int   currentTab:        -1

    Column {
        id: theCol
        spacing: 4

        Repeater {
            model: theHeader.titles
            delegate: Rectangle {
                property bool isCurrent: index == theHeader.currentTab
                width:  theTitle.implicitHeight + 6
                height: theTitle.implicitWidth + 12
                color:  isCurrent ? selectedBgColor : otherBgColor
                Text {
                    id: theTitle
                    rotation:         -90
                    anchors.centerIn: parent
                    text:             modelData
                    color:            isCurrent ? selectedFontColor : otherFontColor
                }
                Rectangle {
                    width: 1
                    color: theTitle.color
                    anchors {
                        right:  parent.right
                        top:    parent.top
                        bottom: parent.bottom
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if( theHeader.currentTab != index )
                            theHeader.currentTab = index
                        else
                            theHeader.currentTab = -1
                    }
                }
            }
        }
    }
}

