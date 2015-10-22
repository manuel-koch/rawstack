import QtQuick 2.0
import QtQuick.Controls 1.3

MenuBar {

    signal openFile()
    signal openDir()
    signal showDevelop()
    signal showExport()

    Menu {
        title: qsTr("&File")

        MenuItem {
            text:        qsTr("Open File")
            shortcut:    "Ctrl+O"
            onTriggered: openFile()
        }

        MenuItem {
            text:        qsTr("Open Directory")
            shortcut:    "Ctrl+Shift+O"
            onTriggered: openDir()
        }

        MenuSeparator { }

        MenuItem {
            text:        qsTr("Quit")
            shortcut:    "Ctrl+Q"
            onTriggered: Qt.quit();
        }
    }

    Menu {
        title: qsTr("&View")

        MenuItem {
            text:        qsTr("Development")
            shortcut:    "Ctrl+1"
            onTriggered: showDevelop()
        }

        MenuItem {
            text:        qsTr("Export")
            shortcut:    "Ctrl+2"
            onTriggered: showExport()
        }
    }
}
