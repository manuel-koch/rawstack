import QtQuick 2.0

Text {
    id: theLabel

    property HistogramLine line

    font.pointSize: 10
    color:          line.color

    states: [
        State {
            name: "left"
            AnchorChanges { target: theLabel; anchors.right: line.left }
            PropertyChanges { target: theLabel; anchors.rightMargin: 2 }
            when: line.x > theLabel.implicitWidth
        },
        State {
            name: "right"
            AnchorChanges { target: theLabel; anchors.left: line.right }
            PropertyChanges { target: theLabel; anchors.leftMargin: 2 }
            when: line.x <= theLabel.implicitWidth
        }
    ]
}

