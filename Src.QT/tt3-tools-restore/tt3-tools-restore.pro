include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_TOOLS_RESTORE_LIBRARY

SOURCES += \
    Component.cpp \
    ConfigureRestoreDialog.cpp \
    Exceptions.cpp \
    RestoreProgressDialog.cpp \
    RestoreReader.cpp \
    RestoreTool.cpp

HEADERS += \
    API.hpp \
    Component.hpp \
    ConfigureRestoreDialog.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    RestoreProgressDialog.hpp \
    RestoreReader.hpp \
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
    ConfigureRestoreDialog.ui \
    RestoreProgressDialog.ui
