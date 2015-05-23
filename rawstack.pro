TEMPLATE = app

QT += qml quick widgets

SOURCES += \
    src/main.cpp \
    src/taskfactory.cpp \
    src/taskbase.cpp \
    src/ufrawtask.cpp \
    src/workerbase.cpp \
    src/configbase.cpp \
    src/ufrawconfig.cpp \
    src/ufrawworker.cpp

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
    src/ufrawworker.h
