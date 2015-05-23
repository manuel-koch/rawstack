import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import "Development" as Development

ApplicationWindow {
    title: qsTr("Rawstack")
    width: 640
    height: 480
    visible: true

    menuBar: MainMenuBar { }

    Development.Main {
        anchors.fill: parent
    }
}
