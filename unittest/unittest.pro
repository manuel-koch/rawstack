TEMPLATE = app
TARGET   = rawstack_unittest
QT      += qml quick widgets xml

include($$PWD/../thirdparty.pri)
include($$PWD/../common.pri)
include($$PWD/../lib/lib.pri)

INCLUDEPATH +=
LIBS        +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

SOURCES += \
    $$GTEST_DIR/src/gtest-all.cc \
    src/main.cpp \
    src/unittestenv.cpp \
    src/testapplicationbase.cpp \
    src/testloghandler.cpp \
    src/testapplication.cpp \
    src/testconfigsetting.cpp \
    src/testconfigsettings.cpp

HEADERS += \
    src/testapplicationbase.h \
    src/testloghandler.h \
    src/testhelpermacros.h

PRE_TARGETDEPS += $$OUT_PWD/../lib/librawstack.a
