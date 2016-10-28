#!/bin/bash
#
# Run macdeployqt to create a self-contained app bundle
# First argument is the build directory that contains the app bundle.
#
QT_BIN_DIR=$HOME/Qt5.7.0/5.7/clang_64/bin
MACDEPLOYQT=$QT_BIN_DIR/macdeployqt
RAWSTACK_APP_DIR=$(dirname $0)
RAWSTACK_APP_BUNDLE=$1/rawstack.app
RAWSTACK_APP_BUNDLE_EXE_DIR=$RAWSTACK_APP_BUNDLE/Contents/MacOS
RAWSTACK_APP_QML_DIR=$RAWSTACK_APP_DIR/qml
UFRAW_EXE=$RAWSTACK_APP_BUNDLE_EXE_DIR/ufraw-batch
ENFUSE_EXE=$RAWSTACK_APP_BUNDLE_EXE_DIR/enfuse

if [ ! -x $MACDEPLOYQT ] ; then
    echo "Deploy tool not executable: $MACDEPLOYQT"
    exit 1
fi
if [ ! -d $RAWSTACK_APP_BUNDLE ] ; then
    echo "Application bundle not found: $RAWSTACK_APP_BUNDLE"
    exit 1
fi

$MACDEPLOYQT $RAWSTACK_APP_BUNDLE -verbose=3 -qmldir=$RAWSTACK_APP_QML_DIR -executable=$UFRAW_EXE -executable=$ENFUSE_EXE
