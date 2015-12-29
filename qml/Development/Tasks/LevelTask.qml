import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Task {
    id: theTask

    name:       "Level"
    controller: LevelController { }

    HistogramEditable {
        controller:   theTask.controller
        width:        parent.width
        height:       width*0.5
        linewidth:    2
        text:         "Input Histogram"
        redEnabled:   false
        greenEnabled: false
        blueEnabled:  false
    }

    TaskSetting {
        label:   "Blackpoint"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          3
            maximumValue:      controller.whitepoint-stepSize
            minimumValue:      0
            stepSize:          0.02
            value:             controller.blackpoint
            onEditingFinished: controller.setBlackpoint( value )
        }
    }

    TaskSetting {
        label:   "Gamma"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          3
            maximumValue:      10
            minimumValue:      0.1
            stepSize:          0.02
            value:             controller.midpoint
            onEditingFinished: controller.setMidpoint( value )
        }
    }

    TaskSetting {
        label:   "Whitepoint"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          3
            maximumValue:      1
            minimumValue:      controller.blackpoint+stepSize
            stepSize:          0.02
            value:             controller.whitepoint
            onEditingFinished: controller.setWhitepoint( value )
        }
    }

    TaskSetting {
        label:   "Brightness"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          3
            maximumValue:      10
            minimumValue:      0
            stepSize:          0.02
            value:             controller.brightness
            onEditingFinished: controller.setBrightness( value )
        }
    }

    TaskSetting {
        label:   "Saturation"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          3
            maximumValue:      10
            minimumValue:      0
            stepSize:          0.02
            value:             controller.saturation
            onEditingFinished: controller.setSaturation( value )
        }
    }
}
