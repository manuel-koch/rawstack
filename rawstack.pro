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

# Add switches for GraphicsMagick to make variables
QMAKE_LIBS     += $$GRAPHICSMAGICK_LIBS
QMAKE_CXXFLAGS += $$GRAPHICSMAGICK_CXXFLAGS
QMAKE_LFLAGS   += $$GRAPHICSMAGICK_LFLAGS

# Need this to work around strange linker errors related to std::string stuff.
# See https://forum.qt.io/topic/33577/trouble-compiling-using-static-library-stk-qt-creator-and-qmake/12
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

QT += qml quick widgets

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
    src/taskstack.cpp

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
    src/taskstack.h
