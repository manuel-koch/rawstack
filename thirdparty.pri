MAC_PORT_BIN_DIR = /opt/local/bin
MAC_PORT_LIB_DIR = /opt/local/lib
MAC_PORT_INC_DIR = /opt/local/include

UFRAW_BINARY_PATH = $$MAC_PORT_BIN_DIR/ufraw-batch
message(Using ufraw at $$UFRAW_BINARY_PATH)

ENFUSE_BINARY_PATH = $$MAC_PORT_BIN_DIR/enfuse
message(Using enfuse at $$ENFUSE_BINARY_PATH)

# Get compiler settings for GraphicsMagick++
GRAPHICSMAGICK_CXXFLAGS = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --cppflags --cxxflags)
GRAPHICSMAGICK_LIBS     = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --libs)
GRAPHICSMAGICK_LFLAGS   = $$system($$MAC_PORT_BIN_DIR/GraphicsMagick++-config --ldflags)
message(GRAPHICSMAGICK_CXXFLAGS = $$GRAPHICSMAGICK_CXXFLAGS)
message(GRAPHICSMAGICK_LFLAGS = $$GRAPHICSMAGICK_LFLAGS)
message(GRAPHICSMAGICK_LIBS = $$GRAPHICSMAGICK_LIBS)

# Library for Exiv2 functionality
EXIV2_LIBS = -lexiv2
