include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_TOOLS_RESTORE_LIBRARY

SOURCES += \
    Component.cpp \
    ConfigureRestoreDialog.cpp \
    Plugins.cpp \
    RestoreTool.cpp

HEADERS += \
    API.hpp \
    Components.hpp \
    ConfigureRestoreDialog.hpp \
    Linkage.hpp \
    RestoreTool.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-tools-restore.qrc

FORMS += \
    ConfigureRestoreDialog.ui
