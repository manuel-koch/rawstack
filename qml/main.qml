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
        id: openFileDialog
        title: "Please choose image file(s) or rawstack configuration(s)"
        selectExisting: true
        selectMultiple: true
        onAccepted: {
            for( var i=0; i<openFileDialog.fileUrls.length; i++ )
            {
                globalConfigDb.add( openFileDialog.fileUrls[i] )
            }
        }
    }

    FileDialog {
        id: openDirDialog
        title: "Please choose directory containing image file(s) or rawstack configuration(s)"
        selectExisting: true
        selectFolder:   true
        onAccepted: {
            for( var i=0; i<openDirDialog.fileUrls.length; i++ )
            {
                globalConfigDb.add( openDirDialog.fileUrls[i] )
            }
        }
    }

    menuBar: MainMenuBar {
        onOpenFile:    openFileDialog.open()
        onOpenDir:     openDirDialog.open()
        onShowDevelop: internal.showView(theDevMain)
        onShowExport:  internal.showView(theExportMain)
    }

    Development.Main {
        id: theDevMain
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
        visible:      false
        anchors.fill: parent
    }

    Binding {
        target:   theAppWindow
        property: "title"
        value:    theExportMain.title
        when:     theExportMain.visible
    }
}
