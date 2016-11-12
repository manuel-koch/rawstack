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

    property real fuse:          task!=undefined ? task.fuse.value          : 0
    property real exposuresigma: task!=undefined ? task.exposuresigma.value : 0
    property real exposure:      task!=undefined ? task.exposure.value      : 0
    property real wbTemperature: task!=undefined ? task.wbTemp.value        : 0
    property real wbGreen:       task!=undefined ? task.wbGreen.value       : 0

    function setFuse(fuse) {
        task.fuse.value = fuse
    }

    function setExposureSigma(sigma) {
        task.exposuresigma.value = sigma
    }

    function setExposure(exposure) {
        task.exposure.value = exposure
    }

    function setWbTemperature(wbTemperature) {
        task.wbTemp.value = wbTemperature
    }

    function setWbGreen(wbGreen) {
        task.wbGreen.value = wbGreen
    }
}

