import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import "Development" as Development

import com.rawstack.types 1.0

ApplicationWindow {
    title: qsTr("Rawstack")
    width: 640
    height: 480
    visible: true

    FileDialog {
        id: openImageFileDialog
        title: "Please choose an image file or rawstack configuration"
        onAccepted: {
            console.log(openImageFileDialog,openImageFileDialog.fileUrls[0])
            globalDevTaskStack.loadFromFile( openImageFileDialog.fileUrls[0] )
        }
    }

    menuBar: MainMenuBar {
        onOpenImage: openImageFileDialog.open()
    }

    Development.Main {
        anchors.fill: parent
    }
}
