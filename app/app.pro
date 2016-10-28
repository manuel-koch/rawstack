TEMPLATE = app
TARGET   = rawstack
QT      += qml quick widgets xml

include(../thirdparty.pri)
include(../common.pri)

# Include thirdparty executables in bundled app
UFRAW_TARGET.files  = $$UFRAW_BINARY_PATH
UFRAW_TARGET.path   = Contents/MacOS
ENFUSE_TARGET.files = $$ENFUSE_BINARY_PATH
ENFUSE_TARGET.path  = Contents/MacOS
QMAKE_BUNDLE_DATA += UFRAW_TARGET ENFUSE_TARGET

SOURCES += \
    src/main.cpp \
    src/taskfactory.cpp \
    src/taskbase.cpp \
    src/ufrawtask.cpp \
    src/workerbase.cpp \
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
    src/stringtoolbox.cpp \
    src/rotatetask.cpp \
    src/rotateworker.cpp \
    src/enfuseprocess.cpp \
    src/imagefactoryworker.cpp \
    src/imagecache.cpp \
    src/loghandler.cpp \
    src/exportimage.cpp \
    src/exportsetting.cpp \
    src/exportqueue.cpp \
    src/configdb.cpp \
    src/configdbentry.cpp \
    src/imagefactorythumbnail.cpp \
    src/imagecacheentry.cpp \
    src/imagecachegroup.cpp \
    src/thumbnailloader.cpp \
    src/imagefactoryfinal.cpp \
    src/configsettings.cpp \
    src/configsetting.cpp \
    src/ufrawsettings.cpp \
    src/rotatesettings.cpp \
    src/sharpensettings.cpp \
    src/sharpentask.cpp \
    src/sharpenworker.cpp \
    src/levelsettings.cpp \
    src/leveltask.cpp \
    src/levelworker.cpp \
    src/configexif.cpp \
    src/histogramdata.cpp \
    src/taskhistogram.cpp \
    src/histogrammaker.cpp \
    src/exporttemplate.cpp \
    src/exportimgtype.cpp \
    src/menumodel.cpp \
    src/menuentry.cpp \
    src/configsettingsstore.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

HEADERS += \
    src/taskfactory.h \
    src/taskbuilderbase.h \
    src/taskbase.h \
    src/ufrawtask.h \
    src/workerbase.h \
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
    src/stringtoolbox.h \
    src/rotatetask.h \
    src/rotateworker.h \
    src/enfuseprocess.h \
    src/imagefactoryworker.h \
    src/imagecache.h \
    src/loghandler.h \
    src/exportimage.h \
    src/exportsetting.h \
    src/exportqueue.h \
    src/configdb.h \
    src/fileinfotoolbox.h \
    src/configdbentry.h \
    src/imagefactorythumbnail.h \
    src/imagecacheentry.h \
    src/imagecachegroup.h \
    src/thumbnailloader.h \
    src/imagefactoryfinal.h \
    src/configsettings.h \
    src/configsetting.h \
    src/ufrawsettings.h \
    src/rotatesettings.h \
    src/sharpensettings.h \
    src/sharpentask.h \
    src/sharpenworker.h \
    src/levelsettings.h \
    src/leveltask.h \
    src/levelworker.h \
    src/configexif.h \
    src/histogramdata.h \
    src/taskhistogram.h \
    src/histogrammaker.h \
    src/exporttemplate.h \
    src/exportimgtype.h \
    src/menumodel.h \
    src/menuentry.h \
    src/configsettingsstore.h
