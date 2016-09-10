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
import QtQuick 2.5

Item {
    id: theContainer

    default property alias containerItems: theContainerItem.children

    property int orientation:  Qt.Horizontal
    property int expandedSize: 300

    QtObject {
        id: internal
        property bool isHoriz: theContainer.orientation == Qt.Horizontal
        property var theConnections: []
        property int currentIndex:   -1
        onCurrentIndexChanged: theContainerItem.updateContentState()

        function onLabelClicked(idx) {
            internal.currentIndex = (internal.currentIndex == idx) ? -1 : idx
        }
    }

    Component.onCompleted: theContainerItem.updateContentState()

    // implicit width of vertical tabs is width of labels when no tab is selected
    Binding {
        target:   theContainer
        property: "implicitWidth"
        value:    theVertLabelLoader.width
        when:     !internal.isHoriz && internal.currentIndex == -1
    }

    // implicit width of vertical tabs is expanded width when tab is selected
    Binding {
        target:   theContainer
        property: "implicitWidth"
        value:    expandedSize
        when:     !internal.isHoriz && internal.currentIndex != -1
    }

    // implicit height of horizontal tabs is height of labels when no tab is selected
    Binding {
        target:   theContainer
        property: "implicitHeight"
        value:    theHorizLabelLoader.height
        when:     internal.isHoriz && internal.currentIndex == -1
    }

    // implicit width of horizontal tabs is expanded height when tab is selected
    Binding {
        target:   theContainer
        property: "implicitHeight"
        value:    expandedSize
        when:     internal.isHoriz && internal.currentIndex != -1
    }

    ListModel {
        id: theTitles
    }

    Loader {
        id: theHorizLabelLoader
        active: internal.isHoriz
        sourceComponent: TabContainerHorizLabels {
            orientation:    theContainer.orientation
            titles:         theTitles
            currentIndex:   internal.currentIndex
            onLabelClicked: internal.onLabelClicked(idx)
        }
    }

    Loader {
        id: theVertLabelLoader
        active: !internal.isHoriz
        sourceComponent: TabContainerVertLabels {
            orientation:    theContainer.orientation
            titles:         theTitles
            currentIndex:   internal.currentIndex
            onLabelClicked: internal.onLabelClicked(idx)
        }
    }

    Item {
        id: theContainerItem
        anchors.top:    theContainer.orientation == Qt.Horizontal ? theHorizLabelLoader.bottom : parent.top
        anchors.bottom: parent.bottom
        anchors.left:   theContainer.orientation == Qt.Horizontal ? parent.left : theVertLabelLoader.right
        anchors.right:  parent.right

        function updateContentState() {
            for( var i=0; i<children.length; i++ ) {
                children[i].state = (i==internal.currentIndex) ? "visible" : "hidden"
                console.debug("TabContainer",i,children[i].state)
            }
        }

        function handleContentTitleChanged(item) {
            for( var i=0; i<children.length; i++ ) {
                if( children[i] == item && i < theTitles.count ) {
                    theTitles.set(i,{"title": item.title})
                    return
                }
            }
        }

        function connectContents() {
            var obj
            while( internal.theConnections.length > children.length ) {
                obj = internal.theConnections.pop()
                obj.destroy(0)
            }
            while( theTitles.count > children.length ) {
                theTitles.remove( theTitles.count-1 )
            }
            for( var i=0; i<children.length; i++ ) {
                var child = children[i]
                child.state = (i==internal.currentIndex) ? "visible" : "hidden"

                if( internal.isHoriz ) {
                    child.anchors.leftMargin  = theHorizLabelLoader.anchors.leftMargin
                    child.anchors.rightMargin = theHorizLabelLoader.anchors.rightMargin
                }
                else {
                    child.anchors.topMargin    = theHorizLabelLoader.anchors.topMargin
                    child.anchors.bottomMargin = theHorizLabelLoader.anchors.bottomMargin
                }

                if( i >= theTitles.count ) {
                    theTitles.append( {"title":child.title} )
                }
                else {
                    theTitles.set( i, {"title":child.title} )
                }

                if( i >= internal.theConnections.length ) {
                    obj = Qt.createQmlObject("import QtQuick 2.0; Connections { ignoreUnknownSignals: true; onTitleChanged: theContainerItem.handleContentTitleChanged(target);}",theContainerItem,"foo")
                    obj.target = child
                    internal.theConnections.push( obj )
                }
                else {
                    internal.theConnections[i].target = child
                }
            }
        }

        onChildrenChanged:     connectContents()
        Component.onCompleted: connectContents()
    }
}
