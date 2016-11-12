TEMPLATE = app
TARGET   = rawstack
QT      += qml quick widgets xml

include($$PWD/../thirdparty.pri)
include($$PWD/../common.pri)
include($$PWD/../lib/lib.pri)

# Include thirdparty executables in bundled app
UFRAW_TARGET.files  = $$UFRAW_BINARY_PATH
UFRAW_TARGET.path   = Contents/MacOS
ENFUSE_TARGET.files = $$ENFUSE_BINARY_PATH
ENFUSE_TARGET.path  = Contents/MacOS
QMAKE_BUNDLE_DATA += UFRAW_TARGET ENFUSE_TARGET

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

SOURCES += src/main.cpp

HEADERS +=

PRE_TARGETDEPS += $$OUT_PWD/../lib/librawstack.a
