import QtQuick 2.0
import QtQuick.Controls 1.3

MenuBar {
    Menu {
        title: qsTr("&File")
        MenuItem {
            text: qsTr("&Quit")
            onTriggered: Qt.quit();
        }
    }
}
