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
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Task {
    id: theTask

    name:       "Ufraw"
    controller: UfrawController { }

    TaskSetting {
        label: "Fuse"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          0
            maximumValue:      9
            minimumValue:      1
            stepSize:          2
            value:             controller.fuse
            onEditingFinished: controller.setFuse( value )
        }
    }

    TaskSetting {
        label: "Fuse Exposure Sigma"
        SpinBox {
            enabled:           controller.fuse > 1
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          2
            maximumValue:      1
            minimumValue:      0.01
            stepSize:          0.01
            value:             controller.exposuresigma
            onEditingFinished: controller.setExposureSigma( value )
        }
    }

    TaskSetting {
        label: "Exposure"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          2
            maximumValue:      8
            minimumValue:      -8
            stepSize:          0.05
            suffix:            "EV"
            value:             controller.exposure
            onEditingFinished: controller.setExposure( value )
        }
    }

    TaskSetting {
        label: "White Balance Temperature"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          0
            maximumValue:      8000
            minimumValue:      0
            stepSize:          50
            suffix:            "°K"
            value:             controller.wbTemperature
            onEditingFinished: controller.setWbTemperature( value )
        }
    }

    TaskSetting {
        label: "White Balance Green"
        SpinBox {
            Layout.alignment:  Qt.AlignRight
            Layout.fillWidth:  true
            font.pointSize:    10
            decimals:          3
            maximumValue:      5
            minimumValue:      0
            stepSize:          0.05
            value:             controller.wbGreen
            onEditingFinished: controller.setWbGreen( value )
        }
    }
}
