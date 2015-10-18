import QtQuick 2.0
import QtQuick.Controls 1.3

MenuBar {

    signal openImage()
    signal showDevelop()
    signal showExport()

    Menu {
        title: qsTr("&File")

        MenuItem {
            text:        qsTr("&Open")
            shortcut:    "Ctrl+O"
            onTriggered: openImage()
        }

        MenuItem {
            text:        qsTr("&Quit")
            shortcut:    "Ctrl+Q"
            onTriggered: Qt.quit();
        }
    }

    Menu {
        title: qsTr("&View")

        MenuItem {
            text:        qsTr("Development")
            onTriggered: showDevelop()
        }

        MenuItem {
            text:        qsTr("Export")
            onTriggered: showExport()
        }
    }
}
