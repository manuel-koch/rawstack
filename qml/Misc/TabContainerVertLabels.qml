import QtQuick 2.5

TabContainerLabels {
    id: theLabels

    anchors.top:         parent.top
    anchors.left:        parent.left
    anchors.leftMargin:  theCol.spacing
    anchors.bottom:      parent.bottom
    width:               theCol.width

    Column {
        id: theCol
        anchors.top:    parent.top
        anchors.left:   parent.left
        anchors.bottom: parent.bottom
        width:          childrenRect.width + spacing
        spacing:        5

        Repeater {
            id: theRepeater
            model: theLabels.titles
            TabContainerLabel {
                orientation: theLabels.orientation
                text:        theLabels.titles.get(index).title
                isActive:    index == currentIndex
                isLast:      (index+1) < theLabels.titles.count
                spacing:     theCol.spacing
                onClicked:   theLabels.labelClicked(index)
            }
        }
    }
}
