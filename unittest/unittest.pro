TEMPLATE = app
TARGET   = rawstack_unittest
QT      += qml quick widgets xml

include($$PWD/../thirdparty.pri)
include($$PWD/../common.pri)
include($$PWD/../lib/lib.pri)

INCLUDEPATH += $$MAC_PORT_INC_DIR/gtest
LIBS        += -L$$MAC_PORT_LIB_DIR -lgtest

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

SOURCES += \
    src/main.cpp \
    src/unittestenv.cpp

HEADERS +=

PRE_TARGETDEPS += $$OUT_PWD/../lib/librawstack.a
