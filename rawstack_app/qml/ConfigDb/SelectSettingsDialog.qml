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
import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import com.rawstack.types 1.0

Dialog {
    id: theDialog
    title:           "Select settings to be applied..."
    standardButtons: StandardButton.Ok | StandardButton.Cancel

    Item {
        implicitWidth:  400
        implicitHeight: 400

        ColumnLayout {
            anchors.fill: parent
            spacing:      4

            Text {
                Layout.fillWidth:       true
                Layout.preferredHeight: implicitHeight
                text:                   theDialog.title
                font.pointSize:         16
            }

            Text {
                Layout.fillWidth:       true
                Layout.preferredHeight: implicitHeight
                text:                   globalConfigStore.nofSelectedSettings + " of " + globalConfigStore.nofSettings + " settings are selected."
            }

            ListView {
                id: theList
                Layout.fillWidth:     true
                Layout.fillHeight:    true
                model:                globalConfigStore
                clip:                 true
                maximumFlickVelocity: 300
                boundsBehavior:       Flickable.StopAtBounds
                delegate: Rectangle {
                    width:  theList.width
                    height: theSettCheckbox.height

                    property var settName:     name
                    property var settValue:    value
                    property var settSelected: selected

                    CheckBox {
                        id: theSettCheckbox
                        checked: settSelected
                        text:    settName + " = " + settValue
                        style: CheckBoxStyle {
                            indicator: Rectangle {
                                implicitWidth:  12
                                implicitHeight: 12
                                x:              2
                                y:              parent.height / 2 - height / 2
                                radius:         3
                                border.color:   "black"

                                Rectangle {
                                    anchors.centerIn: parent
                                    width:   6
                                    height:  6
                                    radius:  2
                                    color:   "black"
                                    visible: theSettCheckbox.checked
                                }
                            }
                        }
                        onCheckedChanged: {
                            console.debug( index, checked )
                            globalConfigStore.setData( globalConfigStore.index(index,0), checked, ConfigSettingsStore.SelectedRole )
                        }
                    }
                }
            }

            Text {
                Layout.fillWidth:       true
                Layout.preferredHeight: implicitHeight
                text:                   "<a href='unselect'>unselect</a> or <a href='select'>select</a> all settings"
                textFormat:             Text.StyledText
                onLinkActivated: {
                    var sel = (link == "select")
                    for( var idx=0; idx<globalConfigStore.nofSettings; idx++ ) {
                        globalConfigStore.setData( globalConfigStore.index(idx,0), sel, ConfigSettingsStore.SelectedRole )
                    }
                }
            }
        }
    }
}
