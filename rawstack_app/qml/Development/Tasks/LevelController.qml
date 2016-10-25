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

TaskController {
    id: theController

    property real blackpoint: task!=undefined ? task.blackpoint.value : 0
    property real midpoint:   task!=undefined ? task.midpoint.value : 0
    property real whitepoint: task!=undefined ? task.whitepoint.value : 0
    property real brightness: task!=undefined ? task.brightness.value : 0
    property real saturation: task!=undefined ? task.saturation.value : 0

    function setBlackpoint(b) {
        task.blackpoint.value = b
    }

    function setMidpoint(m) {
        task.midpoint.value = m
    }

    function setWhitepoint(w) {
        task.whitepoint.value = w
    }

    function setBrightness(b) {
        task.brightness.value = b
    }

    function setSaturation(s) {
        task.saturation.value = s
    }
}

