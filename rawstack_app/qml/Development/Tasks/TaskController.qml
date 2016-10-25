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
import com.rawstack.types 1.0

Item {
    id: theController

    property TaskBase task

    function triggerDevelop(now,force) {
//        console.trace()
//        if( force || theController.task.config.enabled )
//            if( !now )
//                theAggregateTimer.restart()
//            else if( theController.task.dirty )
//                globalDevTaskStack.develop()
    }

    function toggleEnabled() {
        var enabled = !theController.task.enabled
        if( enabled || (!enabled && theController.task.canDisable) ) {
            theController.task.enabled = enabled
            triggerDevelop(true,true)
        }
    }

    Timer {
        id: theAggregateTimer
        repeat:      false
        interval:    300
        onTriggered: if( theController.task.dirty ) globalDevTaskStack.develop()
    }
}

