import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: theHistogram
    width:  100
    height: 100

    QtObject {
        id: internal
        property real toggleSize: 10
        property int  nofEnabled: 4
        property real opacity:    1.0/nofEnabled

        function setToggle(toggle,enabled)
        {
            if( toggle.enabled !== enabled ) {
                toggle.enabled = enabled
                nofEnabled += toggle.enabled ? 1 : -1
                console.log(nofEnabled)
            }
        }

        function toggleChannel(toggle,exclusive)
        {
            var allEnabled = theRedToggle.enabled && theGreenToggle.enabled && theBlueToggle.enabled && theLumToggle.enabled
            if( (exclusive && !allEnabled) || !exclusive )
                setToggle( toggle, !toggle.enabled )
            if( exclusive ) {
                if( toggle !== theRedToggle )
                    internal.setToggle(theRedToggle,!toggle.enabled)
                if( toggle !== theGreenToggle )
                    internal.setToggle(theGreenToggle,!toggle.enabled)
                if( toggle !== theBlueToggle )
                    internal.setToggle(theBlueToggle,!toggle.enabled)
                if( toggle !== theLumToggle )
                    internal.setToggle(theLumToggle,!toggle.enabled)
            }
        }
    }

    HistogramChannel {
        id: theLumChannel
        objectName:   "lum"
        anchors.fill: parent
        bins:         globalDevTaskStack.tasks.final ? globalDevTaskStack.tasks.final.images.histogram.lum : []
        visible:      theLumToggle.enabled
        lineColor:    "black"
        fillColor:    Qt.rgba(0.2,0.2,0.2,0.25)
    }

    HistogramChannel {
        id: theRedChannel
        objectName:   "red"
        anchors.fill: parent
        bins:         globalDevTaskStack.tasks.final ? globalDevTaskStack.tasks.final.images.histogram.red : []
        visible:      theRedToggle.enabled
        lineColor:    "red"
        fillColor:    Qt.rgba(1,0,0,0.25)
    }

    HistogramChannel {
        id: theGreenChannel
        objectName:   "green"
        anchors.fill: parent
        bins:         globalDevTaskStack.tasks.final ? globalDevTaskStack.tasks.final.images.histogram.green : []
        visible:      theGreenToggle.enabled
        lineColor:    "green"
        fillColor:    Qt.rgba(0,1,0,0.25)
    }

    HistogramChannel {
        id: theBlueChannel
        anchors.fill: parent
        objectName:   "blue"
        bins:         globalDevTaskStack.tasks.final ? globalDevTaskStack.tasks.final.images.histogram.blue : []
        visible:      theBlueToggle.enabled
        lineColor:    "blue"
        fillColor:    Qt.rgba(0,0,1,0.25)
    }

    Row {
        id: theToggles
        spacing: 5
        anchors { top: parent.top; right: parent.right; topMargin: spacing; rightMargin: spacing }
        HistogramChannelToggle {
            id: theRedToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  "red"
            onToggleChannel: internal.toggleChannel(theRedToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
        HistogramChannelToggle {
            id: theGreenToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  "green"
            onToggleChannel: internal.toggleChannel(theGreenToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
        HistogramChannelToggle {
            id: theBlueToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  "blue"
            onToggleChannel: internal.toggleChannel(theBlueToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
        HistogramChannelToggle {
            id: theLumToggle
            width:  internal.toggleSize
            height: internal.toggleSize
            radius: 2
            color:  Qt.rgba(0.9,0.9,0.9)
            onToggleChannel: internal.toggleChannel(theLumToggle,exclusive)
            onEnabledChanged: console.log(color,enabled)
        }
    }

    Text {
        id: theHint
        anchors.centerIn: parent
        horizontalAlignment: Qt.AlignHCenter
    }
}

