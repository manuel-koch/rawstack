#!/bin/bash
#
# Run macdeployqt to create a self-contained app bundle
#
QT_BIN_DIR=$HOME/Qt5.7.0/5.7/clang_64/bin
MACDEPLOYQT=$QT_BIN_DIR/macdeployqt
RAWSTACK_DIR=$(dirname $0)
RAWSTACK_BUILD_DIR=$RAWSTACK_DIR/build/debug
RAWSTACK_APP=$RAWSTACK_BUILD_DIR/rawstack.app
RAWSTACK_QML_DIR=$RAWSTACK_DIR/qml
BUNDLE_EXE_DIR=$RAWSTACK_BUILD_DIR/rawstack.app/Contents/MacOS
UFRAW_EXE=$BUNDLE_EXE_DIR/ufraw-batch
ENFUSE_EXE=$BUNDLE_EXE_DIR/enfuse

$MACDEPLOYQT $RAWSTACK_APP -verbose=3 -qmldir=$RAWSTACK_QML_DIR -executable=$UFRAW_EXE -executable=$ENFUSE_EXE
