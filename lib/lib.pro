TEMPLATE = lib
TARGET   = rawstack
QT      -= gui
QT      += qml quick widgets xml

CONFIG += staticlib

include($$PWD/../thirdparty.pri)
include($$PWD/../common.pri)

RESOURCES += \
    rawstack.qrc

SOURCES += \
    src/commontasks.cpp \
    src/configdb.cpp \
    src/configdbentry.cpp \
    src/configexif.cpp \
    src/configsetting.cpp \
    src/configsettings.cpp \
    src/configsettingsstore.cpp \
    src/enfuseprocess.cpp \
    src/exportimage.cpp \
    src/exportimgtype.cpp \
    src/exportqueue.cpp \
    src/exportsetting.cpp \
    src/exporttemplate.cpp \
    src/histogramdata.cpp \
    src/histogrammaker.cpp \
    src/imagecache.cpp \
    src/imagecacheentry.cpp \
    src/imagecachegroup.cpp \
    src/imagefactory.cpp \
    src/imagefactorybase.cpp \
    src/imagefactoryfinal.cpp \
    src/imagefactoryoverexposed.cpp \
    src/imagefactoryregistry.cpp \
    src/imagefactorythumbnail.cpp \
    src/imagefactoryunderexposed.cpp \
    src/imagefactoryworker.cpp \
    src/imageprovider.cpp \
    src/levelsettings.cpp \
    src/leveltask.cpp \
    src/levelworker.cpp \
    src/librawstack.cpp \
    src/loghandler.cpp \
    src/menuentry.cpp \
    src/menumodel.cpp \
    src/rotatesettings.cpp \
    src/rotatetask.cpp \
    src/rotateworker.cpp \
    src/sharpensettings.cpp \
    src/sharpentask.cpp \
    src/sharpenworker.cpp \
    src/stringtoolbox.cpp \
    src/taskbase.cpp \
    src/taskfactory.cpp \
    src/taskhistogram.cpp \
    src/taskstack.cpp \
    src/thumbnailloader.cpp \
    src/ufrawprocess.cpp \
    src/ufrawsettings.cpp \
    src/ufrawtask.cpp \
    src/ufrawworker.cpp \
    src/workerbase.cpp

HEADERS += \
    src/commontasks.h \
    src/configdb.h \
    src/configdbentry.h \
    src/configexif.h \
    src/configsetting.h \
    src/configsettings.h \
    src/configsettingsstore.h \
    src/enfuseprocess.h \
    src/exportimage.h \
    src/exportimgtype.h \
    src/exportqueue.h \
    src/exportsetting.h \
    src/exporttemplate.h \
    src/fileinfotoolbox.h \
    src/histogramdata.h \
    src/histogrammaker.h \
    src/imagecache.h \
    src/imagecacheentry.h \
    src/imagecachegroup.h \
    src/imagefactory.h \
    src/imagefactorybase.h \
    src/imagefactoryfinal.h \
    src/imagefactoryoverexposed.h \
    src/imagefactoryregistry.h \
    src/imagefactorythumbnail.h \
    src/imagefactoryunderexposed.h \
    src/imagefactoryworker.h \
    src/imageprovider.h \
    src/levelsettings.h \
    src/leveltask.h \
    src/levelworker.h \
    src/librawstack.h \
    src/loghandler.h \
    src/menuentry.h \
    src/menumodel.h \
    src/rotatesettings.h \
    src/rotatetask.h \
    src/rotateworker.h \
    src/sharpensettings.h \
    src/sharpentask.h \
    src/sharpenworker.h \
    src/stringtoolbox.h \
    src/taskbase.h \
    src/taskbuilder.h \
    src/taskbuilderbase.h \
    src/taskfactory.h \
    src/taskhistogram.h \
    src/taskstack.h \
    src/thumbnailloader.h \
    src/ufrawprocess.h \
    src/ufrawsettings.h \
    src/ufrawtask.h \
    src/ufrawworker.h \
    src/workerbase.h
