import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Task {
    id: theTask

    name:       "Ufraw"
    controller: UfrawController { }

    TaskSetting {
        label:   "Fuse"
        SpinBox {
            Layout.alignment:      Qt.AlignRight
            Layout.preferredWidth: parent.width/2
            decimals:          0
            maximumValue:      9
            minimumValue:      1
            stepSize:          2
            value:             controller.fuse
            onEditingFinished: controller.setFuse( value )
        }
    }

    TaskSetting {
        label:   "Exposure"
        SpinBox {
            Layout.alignment:      Qt.AlignRight
            Layout.preferredWidth: parent.width/2
            decimals:          2
            maximumValue:      8
            minimumValue:      -8
            stepSize:          0.05
            suffix:            "EV"
            value:             controller.exposure
            onEditingFinished: controller.setExposure( value )
        }
    }
}
