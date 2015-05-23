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

    Image {
        id: theLumChannel
        anchors.fill: parent
        fillMode:     Image.Stretch
//        source:       theDevCtrl.chain.images.histogramLum.url
        visible:      theLumToggle.enabled
        opacity:      internal.opacity
        onStatusChanged: {
            if( status != Image.Ready )
                theHint.text = "Histogram not\navailable yet"
            else
                theHint.text = ""
        }
    }

    Image {
        id: theRedChannel
        anchors.fill: parent
        fillMode:     Image.Stretch
//        source:       theDevCtrl.chain.images.histogramRed.url
        visible:      theRedToggle.enabled
        opacity:      internal.opacity
    }

    Image {
        id: theGreenChannel
        anchors.fill: parent
        fillMode:     Image.Stretch
//        source:       theDevCtrl.chain.images.histogramGreen.url
        visible:      theGreenToggle.enabled
        opacity:      internal.opacity
    }

    Image {
        id: theBlueChannel
        anchors.fill: parent
        fillMode:     Image.Stretch
//        source:       theDevCtrl.chain.images.histogramBlue.url
        visible:      theBlueToggle.enabled
        opacity:      internal.opacity
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
            color:  "silver"
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

