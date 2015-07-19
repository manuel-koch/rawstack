import QtQuick 2.0
import QtQuick.Controls 1.3

MenuBar {

    signal openImage()

    Menu {
        title: qsTr("&File")

        MenuItem {
            text: qsTr("&Open image")
            onTriggered: openImage()
        }

        MenuItem {
            text: qsTr("&Quit")
            onTriggered: Qt.quit();
        }
    }
}
