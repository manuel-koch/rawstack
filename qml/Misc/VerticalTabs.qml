import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: theTabs

    width:  theHeader.width + ( (currentTab != -1) ? 4 + theTabContainer.width : 0 )
    height: 400

    property alias currentTab: theHeader.currentTab

    default property alias containerItems: theTabContainer.children

    VerticalTabHeader {
        id: theHeader
        anchors {
            top:    parent.top
            bottom: parent.bottom
            left:   parent.left
        }
    }

    Item {
        id: theTabContainer
        anchors {
            top:        parent.top
            bottom:     parent.bottom
            left:       theHeader.right
            leftMargin: 4
        }
        visible: theTabs.currentTab != -1

        onHeightChanged: console.debug(theTabContainer,height)

        onChildrenChanged: {
            var titles = []
            var child
            for(var i=0; i<children.length; i++ ) {
                child = children[i]
                titles.push( child.title )
            }
            theHeader.titles = titles
        }
    }

    Binding {
        target:   theTabContainer
        property: "width"
        value:    theTabs.currentTab != -1 ? theTabContainer.children[theTabs.currentTab].width : 0
        when:     theTabs.currentTab != -1
    }

    Connections {
        target: theHeader
        onCurrentTabChanged: {
            for( var i=0; i<theTabContainer.children.length; i++ ) {
                theTabContainer.children[i].visible = (i == theTabs.currentTab)
            }
        }
    }
}

