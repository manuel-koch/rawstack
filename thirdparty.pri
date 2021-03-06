MAC_PORT_BIN_DIR = /opt/local/bin
MAC_PORT_LIB_DIR = /opt/local/lib
MAC_PORT_INC_DIR = /opt/local/include

UFRAW_BINARY_PATH = $$MAC_PORT_BIN_DIR/ufraw-batch
!exists( $$UFRAW_BINARY_PATH ) {
    error(Could not find ufraw at $$UFRAW_BINARY_PATH)
}
UFRAW_BINARY_VERSION = $$system($$UFRAW_BINARY_PATH --version 2>&1 | head -1)
UFRAW_BINARY_VERSION = $$last(UFRAW_BINARY_VERSION)
message(Using ufraw $$UFRAW_BINARY_VERSION at $$UFRAW_BINARY_PATH)

ENFUSE_BINARY_PATH = $$MAC_PORT_BIN_DIR/enfuse
!exists( $$ENFUSE_BINARY_PATH ) {
    error(Could not find enfuse at $$ENFUSE_BINARY_PATH)
}
ENFUSE_BINARY_VERSION = $$system($$ENFUSE_BINARY_PATH --version 2>&1 | head -1)
ENFUSE_BINARY_VERSION = $$last(ENFUSE_BINARY_VERSION)
message(Using enfuse $$ENFUSE_BINARY_VERSION at $$ENFUSE_BINARY_PATH)

# Get compiler settings for GraphicsMagick++
GRAPHICSMAGICK_VERSION = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --version)
GRAPHICSMAGICK_CXXFLAGS = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --cppflags --cxxflags)
GRAPHICSMAGICK_LIBS     = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --libs)
GRAPHICSMAGICK_LFLAGS   = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --ldflags)
message(Using GraphicsMagick $$GRAPHICSMAGICK_VERSION)
message(GRAPHICSMAGICK_CXXFLAGS = $$GRAPHICSMAGICK_CXXFLAGS)
message(GRAPHICSMAGICK_LFLAGS = $$GRAPHICSMAGICK_LFLAGS)
message(GRAPHICSMAGICK_LIBS = $$GRAPHICSMAGICK_LIBS)

# Library for Exiv2 functionality
EXIV2_LIBS = -lexiv2

# Test framework gtest/gmock
GTEST_DIR = $$PWD/../googletest/googletest
GMOCK_DIR = $$PWD/../googletest/googlemock
