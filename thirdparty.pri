UFRAW_BINARY = /opt/local/bin/ufraw-batch
!exists($$UFRAW_BINARY) {
    message(Unable to find binary UFRAW_BINARY at $$UFRAW_BINARY)
}
else {
    message(Using UFRAW_BINARY at $$UFRAW_BINARY)
}
UFRAW_DATA.files = $$UFRAW_BINARY
UFRAW_DATA.path  = Contents/MacOS

ENFUSE_BINARY = /opt/local/bin/enfuse
!exists($$ENFUSE_BINARY) {
    message(Unable to find binary ENFUSE_BINARY at $$ENFUSE_BINARY)
}
else {
    message(Using ENFUSE_BINARY at $$ENFUSE_BINARY)
}
ENFUSE_DATA.files = $$ENFUSE_BINARY
ENFUSE_DATA.path  = Contents/MacOS

QMAKE_BUNDLE_DATA += UFRAW_DATA ENFUSE_DATA
