import QtQuick 2.5

Item {
    id: theContent

    /// @brief The title of tab
    property string title

    /// @brief Request selected expanded size of content when this tab is shown.
    ///        Using zero size will result in default size of tab container.
    property int expandedSize: 0

    anchors.fill: parent

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target:  theContent
                opacity: 0
                visible: false
            }
        },
        State {
            name: "visible"
            PropertyChanges {
                target:  theContent
                opacity: 1.0
                visible: true
            }
        }
    ]

    transitions: [
        Transition {
            from: "visible"
            to:   "hidden"
            FadeOutAnimation { target: theContent }
        },
        Transition {
            from: "hidden"
            to:   "visible"
            FadeInAnimation { target: theContent }
      }
    ]
}
