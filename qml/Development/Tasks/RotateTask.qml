import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Task {
    id: theTask

    name:       "Rotate"
    controller: RotateController { }

    TaskSetting {
        label:   "Degree"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          2
            maximumValue:      360
            minimumValue:      -360
            stepSize:          1
            suffix:            "°"
            value:             controller.degree
            onEditingFinished: controller.setDegree( value )
        }
    }
}
