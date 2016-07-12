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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

import com.rawstack.types 1.0
import "../Misc" as Misc
import "../Export" as Export

Misc.VerticalTabContent {
    id: theExportTab
    title: "Export"

    ColumnLayout {
        id: theColLayout
        width:          300
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        onHeightChanged: console.debug(theColLayout,height)

        Text {
            text: "Export Settings..."
        }

        Item {
            height: theColLayout.spacing
        }

        Text {
            font.pointSize: 10
            text: "Output Path :"
        }

        TextField {
            id: theImgPathEdit
            Layout.fillWidth: true
            font.pointSize:   10

            onEditingFinished: {
                globalExportTemplate.imgPath = theImgPathEdit.text
            }

            function syncImgPath() {
                theImgPathEdit.text = globalExportTemplate.imgPath
            }

            Component.onCompleted: {
                theImgPathEdit.syncImgPath()
            }
        }

        Text {
            text: "Image Type :"
        }

        ComboBox {
            id: theImgTypeCombo
            Layout.fillWidth: true
            style: ComboBoxStyle {
                font.pointSize: 10
            }
            model: ListModel {
                    id: typeItems
                    ListElement { text: "JPG"; imgtype: ExportImgType.JPG }
                    ListElement { text: "TIF"; imgtype: ExportImgType.TIF }
                    ListElement { text: "PNG"; imgtype: ExportImgType.PNG }
            }

            onCurrentIndexChanged: {
                globalExportTemplate.imgType = typeItems.get(currentIndex).imgtype
            }

            function syncImgType() {
                for( var i=0; i<typeItems.count; i++ ) {
                    if( typeItems.get(i).imgtype == globalExportTemplate.imgType )
                        theImgTypeCombo.currentIndex = i
                        break
                }
            }

            Component.onCompleted: {
                theImgTypeCombo.syncImgType()
            }
        }

        Text {
            text: "Image Quality :"
            font.pointSize: 10
        }

        SpinBox {
            id: theImgQualitySlider
            Layout.fillWidth: true
            minimumValue:     30
            maximumValue:     100
            stepSize:         1
            decimals:         0
            font.pointSize:   10

            onEditingFinished: {
                globalExportTemplate.imgQuality = value
            }

            function syncImgQuality() {
                theImgQualitySlider.value = globalExportTemplate.imgQuality
            }

            Component.onCompleted: {
                theImgQualitySlider.syncImgQuality()
            }
        }

        Text {
            text: "Export Queue :"
            font.pointSize: 10
        }

        Export.ExportQueue {
            id: theQueue
            Layout.fillWidth:  true
            Layout.fillHeight: true
        }
    }

    Connections {
        target: globalExportTemplate
        onImgPathChanged:    theImgPathEdit.syncImgPath()
        onImgTypeChanged:    theImgTypeCombo.syncImgType()
        onImgQualityChanged: theImgQualitySlider.syncImgQuality()
    }
}
