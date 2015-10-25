TEMPLATE = app

# Use latest C++ standard to enable lambdas
QMAKE_CXXFLAGS += -std=c++11

# Get compiler settings for GraphicsMagick++
GRAPHICSMAGICK_CXXFLAGS = $$system(/opt/local/bin/GraphicsMagick++-config --cppflags --cxxflags)
GRAPHICSMAGICK_LIBS     = $$system(/opt/local/bin/GraphicsMagick++-config --libs)
GRAPHICSMAGICK_LFLAGS   = $$system(/opt/local/bin/GraphicsMagick++-config --ldflags)
message(GRAPHICSMAGICK_CXXFLAGS = $$GRAPHICSMAGICK_CXXFLAGS)
message(GRAPHICSMAGICK_LFLAGS = $$GRAPHICSMAGICK_LFLAGS)
message(GRAPHICSMAGICK_LIBS = $$GRAPHICSMAGICK_LIBS)

EXIV2_LIBS = -lexiv2

# Add switches for GraphicsMagick to make variables
QMAKE_LIBS     += $$GRAPHICSMAGICK_LIBS
QMAKE_CXXFLAGS += $$GRAPHICSMAGICK_CXXFLAGS
QMAKE_LFLAGS   += $$GRAPHICSMAGICK_LFLAGS $$EXIV2_LIBS

# Add path to mac ports includes
QMAKE_CXXFLAGS += -I/opt/local/include

# Need to tell Qt which Xcode SDK is installed
# See https://forum.qt.io/topic/58926/solved-xcode-7-and-qt-error/3
QMAKE_MAC_SDK = macosx10.11

# Need this to work around strange linker errors related to std::string stuff.
# See https://forum.qt.io/topic/33577/trouble-compiling-using-static-library-stk-qt-creator-and-qmake/12
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

QT += qml quick widgets xml

SOURCES += \
    src/main.cpp \
    src/taskfactory.cpp \
    src/taskbase.cpp \
    src/ufrawtask.cpp \
    src/workerbase.cpp \
    src/configbase.cpp \
    src/ufrawconfig.cpp \
    src/ufrawworker.cpp \
    src/ufrawprocess.cpp \
    src/taskstack.cpp \
    src/imagefactory.cpp \
    src/imagefactorybase.cpp \
    src/imageprovider.cpp \
    src/imagefactoryoverexposed.cpp \
    src/imagefactoryunderexposed.cpp \
    src/imagefactoryregistry.cpp \
    src/commontasks.cpp \
    src/configsaverbase.cpp \
    src/stringtoolbox.cpp \
    src/configxmlsaver.cpp \
    src/configfilesaver.cpp \
    src/configloaderbase.cpp \
    src/configxmlloader.cpp \
    src/configfileloader.cpp \
    src/histfactory.cpp \
    src/rotateconfig.cpp \
    src/rotatetask.cpp \
    src/rotateworker.cpp \
    src/enfuseprocess.cpp \
    src/imagefactoryworker.cpp \
    src/imagefactorypreview.cpp \
    src/imagecache.cpp \
    src/loghandler.cpp \
    src/commonconfig.cpp \
    src/exportimage.cpp \
    src/exportsetting.cpp \
    src/exportqueue.cpp \
    src/configdb.cpp \
    src/configdbentry.cpp \
    src/imagefactorythumbnail.cpp \
    src/imagecacheentry.cpp \
    src/imagecachegroup.cpp \
    src/thumbnailloader.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/taskfactory.h \
    src/taskbuilderbase.h \
    src/taskbase.h \
    src/ufrawtask.h \
    src/workerbase.h \
    src/configbase.h \
    src/ufrawconfig.h \
    src/ufrawworker.h \
    src/ufrawprocess.h \
    src/taskbuilder.h \
    src/taskstack.h \
    src/imagefactory.h \
    src/imagefactorybase.h \
    src/imageprovider.h \
    src/imagefactoryoverexposed.h \
    src/imagefactoryunderexposed.h \
    src/imagefactoryregistry.h \
    src/commontasks.h \
    src/configsaverbase.h \
    src/stringtoolbox.h \
    src/configxmlsaver.h \
    src/configfilesaver.h \
    src/configbuilder.h \
    src/configbuilderbase.h \
    src/configloaderbase.h \
    src/configxmlloader.h \
    src/configfileloader.h \
    src/histfactory.h \
    src/rotateconfig.h \
    src/rotatetask.h \
    src/rotateworker.h \
    src/enfuseprocess.h \
    src/imagefactoryworker.h \
    src/imagefactorypreview.h \
    src/imagecache.h \
    src/loghandler.h \
    src/commonconfig.h \
    src/exportimage.h \
    src/exportsetting.h \
    src/exportqueue.h \
    src/configdb.h \
    src/fileinfotoolbox.h \
    src/configdbentry.h \
    src/imagefactorythumbnail.h \
    src/imagecacheentry.h \
    src/imagecachegroup.h \
    src/thumbnailloader.h
