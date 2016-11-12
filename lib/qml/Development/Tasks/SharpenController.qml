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

    property real radius:    task!=undefined ? task.radius.value    : 0
    property real sigma:     task!=undefined ? task.sigma.value     : 0
    property real threshold: task!=undefined ? task.threshold.value : 0
    property real amount:    task!=undefined ? task.amount.value    : 0

    function setRadius(radius) {
        task.radius.value = radius
    }

    function setSigma(sigma) {
        task.sigma.value = sigma
    }

    function setThreshold(threshold) {
        task.threshold.value = threshold
    }

    function setAmount(amount) {
        task.amount.value = amount
    }
}

