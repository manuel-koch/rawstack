import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import com.rawstack.types 1.0
import "../Misc" as Misc
import "../Export" as Export

Misc.TabContent {
    title: "Export"

    Rectangle {
        color:        "transparent"
        border.color: "#a0a0a0"
        border.width: 1

        width:        200
        height:       200

        ColumnLayout {
            anchors.fill:    parent
            anchors.margins: 4

            Text {
                text: "Export Settings..."
            }

            RowLayout {
                anchors {
                    left:   parent.left
                    right:  parent.right
                }
                Layout.preferredHeight: theImgTypeCombo.height

                Text {
                    Layout.fillHeight:     true
                    Layout.preferredWidth: parent.width/2
                    verticalAlignment:     Qt.AlignVCenter
                    text: "Image Type :"
                }

                ComboBox {
                    id: theImgTypeCombo
                    Layout.fillWidth: true
                    model: ListModel {
                            id: typeItems
                            ListElement { text: "JPG"; imgtype: ExportImgType.JPG }
                            ListElement { text: "TIF"; imgtype: ExportImgType.TIF }
                            ListElement { text: "PNG"; imgtype: ExportImgType.PNG }
                    }
                    onCurrentIndexChanged: {
                        globalExportTemplate.imgType = typeItems.get(currentIndex).imgtype
                    }
                }
            }

            Connections {
                target: globalExportTemplate
                onImgTypeChanged: {
                    for( var i=0; i<typeItems.count; i++ ) {
                        if( typeItems.get(i).imgtype == globalExportTemplate.imgType )
                            theImgTypeCombo.currentIndex = i
                            break
                    }
                }
            }

            RowLayout {
                anchors {
                    left:   parent.left
                    right:  parent.right
                }
                Layout.preferredHeight: theImgQualitySlider.height

                Text {
                    Layout.fillHeight:     true
                    Layout.preferredWidth: parent.width/2
                    verticalAlignment:     Qt.AlignVCenter
                    text:                  "Image Quality :"
                }

                SpinBox {
                    id: theImgQualitySlider
                    Layout.fillWidth: true
                    minimumValue:     30
                    maximumValue:     100
                    stepSize:         0.2
                    decimals:         1
                    onEditingFinished: {
                        globalExportTemplate.imgQuality = value
                    }
                }
            }

            Connections {
                target: globalExportTemplate
                onImgQualityChanged: {
                    theImgQualitySlider.value = globalExportTemplate.imgQuality
                }
            }

            Export.ExportQueue {
                Layout.fillWidth:  true
                Layout.fillHeight: true
            }
        }
    }
}
