import QtQuick 2.5

TabContainerLabels {
    id: theLabels

    anchors.top:         parent.top
    anchors.topMargin:   theRow.spacing
    anchors.left:        parent.left
    anchors.right:       parent.right
    height:              theRow.height

    Row {
        id: theRow
        anchors.top:   parent.top
        anchors.left:  parent.left
        anchors.right: parent.right
        height:        childrenRect.height + spacing
        spacing:       5

        Repeater {
            id: theRepeater
            model: theLabels.titles
            TabContainerLabel {
                orientation: theLabels.orientation
                text:        theLabels.titles.get(index).title
                isActive:    index == currentIndex
                isLast:      (index+1) < theLabels.titles.count
                spacing:     theRow.spacing
                onClicked:   theLabels.labelClicked(index)
            }
        }
    }
}
