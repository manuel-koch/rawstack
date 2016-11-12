# Use latest C++ standard to enable lambdas
QMAKE_CXXFLAGS += -std=c++11

# Add switches for GraphicsMagick to make variables
QMAKE_LIBS     += $$GRAPHICSMAGICK_LIBS
QMAKE_CXXFLAGS += $$GRAPHICSMAGICK_CXXFLAGS
QMAKE_LFLAGS   += $$GRAPHICSMAGICK_LFLAGS $$EXIV2_LIBS

# Add path to mac ports includes
QMAKE_CXXFLAGS += -I$$MAC_PORT_INC_DIR

# Need to tell Qt which Xcode SDK is installed
# See https://forum.qt.io/topic/58926/solved-xcode-7-and-qt-error/3
# Try the following command to get a list of supported SDKs
# $ xcodebuild -showsdks
QMAKE_MAC_SDK = macosx10.12

# Need this to work around strange linker errors related to std::string stuff.
# See https://forum.qt.io/topic/33577/trouble-compiling-using-static-library-stk-qt-creator-and-qmake/12
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
