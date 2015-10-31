import QtQuick 2.0
import QtQuick.Controls 1.3

MenuBar {

    signal openFile()
    signal openDir()

    signal loadDb()
    signal saveDb()
    signal saveAsDb()

    signal showOverview()
    signal showDevelop()
    signal showExport()

    Menu {
        title: qsTr("&File")

        MenuItem {
            text:        qsTr("Add image...")
            shortcut:    "Ctrl+Shift+O"
            onTriggered: openFile()
        }

        MenuItem {
            text:        qsTr("Add directory...")
            shortcut:    "Ctrl+Alt+O"
            onTriggered: openDir()
        }

        MenuSeparator { }

        MenuItem {
            text:        qsTr("Load database...")
            shortcut:    "Ctrl+O"
            onTriggered: loadDb()
        }

        MenuItem {
            text:        qsTr("Save database...")
            shortcut:    "Ctrl+S"
            enabled:     globalConfigDb.path
            onTriggered: saveDb()
        }

        MenuItem {
            text:        qsTr("Save database as...")
            shortcut:    "Ctrl+Shift+S"
            onTriggered: saveAsDb()
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
            text:        qsTr("Overview")
            shortcut:    "Ctrl+1"
            onTriggered: showOverview()
        }

        MenuItem {
            text:        qsTr("Development")
            shortcut:    "Ctrl+2"
            onTriggered: showDevelop()
        }

        MenuItem {
            text:        qsTr("Export")
            shortcut:    "Ctrl+3"
            onTriggered: showExport()
        }
    }
}
