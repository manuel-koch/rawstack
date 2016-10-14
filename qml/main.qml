/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
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
    title:   "Rawstack"
    width:   600
    height:  600
    visible: true

    function centerOnScreen() {
        theAppWindow.x = Screen.desktopAvailableWidth/2 - theAppWindow.width/2
        theAppWindow.y = Screen.desktopAvailableHeight/2 - theAppWindow.height/2
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

    FileDialog {
        id: loadDbDialog
        title: "Please choose database"
        selectExisting: true
        onAccepted: {
            globalConfigDb.load( loadDbDialog.fileUrls[0] )
        }
    }

    FileDialog {
        id: saveDbDialog
        title: "Select path to database"
        selectExisting: false
        selectMultiple: false
        onAccepted: {
            globalConfigDb.save( saveDbDialog.fileUrls[0] )
        }
    }

    menuBar: MainMenuBar {
        onOpenFile:     openFileDialog.open()
        onOpenDir:      openDirDialog.open()
        onLoadDb:       loadDbDialog.open()
        onSaveDb:       globalConfigDb.save()
        onSaveAsDb:     saveDbDialog.open()
    }

    Overview.Main {
        id: theOverviewMain
        anchors.fill: parent
        visible:      globalMenuModel.screenOverview.checked
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
        visible:      globalMenuModel.screenDevelop.checked
    }

    Binding {
        target:   theAppWindow
        property: "title"
        value:    theDevMain.title
        when:     theDevMain.visible
    }
}
