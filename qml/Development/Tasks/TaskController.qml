import QtQuick 2.0
import com.rawstack.types 1.0

Item {
    id: theController

    property TaskBase task

    function triggerDevelop(now,force) {
        console.trace()
        if( force || theController.task.config.enabled )
            if( !now )
                theAggregateTimer.restart()
            else if( theController.task.config.dirty )
                globalDevTaskStack.develop()
    }

    function toggleEnabled() {
        console.log(theController,task.config.enabled,theController.task.config.canDisable)
        var enabled = !theController.task.config.enabled
        if( enabled || (!enabled && theController.task.config.canDisable) ) {
            theController.task.config.enabled = enabled
            triggerDevelop(true,true)
        }
    }

    Timer {
        id: theAggregateTimer
        repeat:      false
        interval:    300
        onTriggered: if( theController.task.config.dirty ) globalDevTaskStack.develop()
    }
}

