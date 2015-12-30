import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Task {
    id: theTask

    name:       "Sharpen"
    controller: SharpenController { }

    TaskSetting {
        label:   "Radius"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          0
            maximumValue:      200
            minimumValue:      0
            stepSize:          1
            value:             controller.radius
            onEditingFinished: controller.setRadius( value )
        }
    }

    TaskSetting {
        label:   "Sigma"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          2
            maximumValue:      10
            minimumValue:      0.01
            stepSize:          0.02
            value:             controller.sigma
            onEditingFinished: controller.setSigma( value )
        }
    }
}
