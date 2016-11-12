TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    lib \
    app \
    unittest

app.depends = lib
unittest.depends = lib
