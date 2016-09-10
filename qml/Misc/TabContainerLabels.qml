import QtQuick 2.5

Item {
    id: theLabels

    property int orientation:  Qt.Horizontal
    property int currentIndex: -1
    property ListModel titles

    signal labelClicked(int idx)
}
