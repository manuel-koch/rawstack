import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import "Overview" as Overview
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
            if( main === theOverviewMain ) {
                theDevMain.visible      = false
                theExportMain.visible   = false
                theOverviewMain.visible = true
            }
            else if( main === theDevMain ) {
                theOverviewMain.visible = false
                theExportMain.visible   = false
                theDevMain.visible      = true
            }
            else if( main === theExportMain ) {
                theOverviewMain.visible = false
                theDevMain.visible      = false
                theExportMain.visible   = true
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
        onOpenFile:     openFileDialog.open()
        onOpenDir:      openDirDialog.open()
        onShowOverview: internal.showView(theOverviewMain)
        onShowDevelop:  internal.showView(theDevMain)
        onShowExport:   internal.showView(theExportMain)
    }

    Overview.Main {
        id: theOverviewMain
        anchors.fill: parent
    }

    Binding {
        target:   theAppWindow
        property: "title"
        value:    theOverviewMain.title
        when:     theOverviewMain.visible
    }

    Development.Main {
        id: theDevMain
        anchors.fill: parent
        visible:      false
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
        visible:      false
    }

    Binding {
        target:   theAppWindow
        property: "title"
        value:    theExportMain.title
        when:     theExportMain.visible
    }
}
