import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import "Development" as Development
import "Export" as Export

import com.rawstack.types 1.0

ApplicationWindow {
    id: theAppWindow
    title: "Rawstack"
    width: 640
    height: 480
    visible: true

    QtObject {
        id: internal

        function showView(main) {
            if( main === theDevMain ) {
                theExportMain.visible = false
                theDevMain.visible    = true
            }
            else if( main === theExportMain ) {
                theDevMain.visible    = false
                theExportMain.visible = true
            }
        }
    }

    FileDialog {
        id: openImageFileDialog
        title: "Please choose an image file or rawstack configuration"
        onAccepted: {
            console.log(openImageFileDialog,openImageFileDialog.fileUrls[0])
            globalDevTaskStack.loadFromFile( openImageFileDialog.fileUrls[0] )
        }
    }

    menuBar: MainMenuBar {
        onOpenImage:   openImageFileDialog.open()
        onShowDevelop: internal.showView(theDevMain)
        onShowExport:  internal.showView(theExportMain)
    }

    Development.Main {
        id: theDevMain
        visible: false
        anchors.fill: parent
    }

    Binding {
        target:   theAppWindow
        property: "title"
        value:    theDevMain.title
        when:     theDevMain.visible
    }

    Export.Main {
        id: theExportMain
        anchors.fill: parent
    }

    Binding {
        target:   theAppWindow
        property: "title"
        value:    theExportMain.title
        when:     theExportMain.visible
    }
}
