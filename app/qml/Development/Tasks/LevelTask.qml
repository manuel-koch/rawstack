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
