include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_TOOLS_BACKUP_LIBRARY
SOURCES += \
    BackupTool.cpp \
    BackupWriter.cpp \
    Component.cpp \
    ConfigureBackupDialog.cpp \
    Plugins.cpp

HEADERS += \
    API.hpp \
    BackupTool.hpp \
    BackupWriter.hpp \
    Components.hpp \
    ConfigureBackupDialog.hpp \
    Linkage.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    tt3-tools-backup.qrc

FORMS += \
    ConfigureBackupDialog.ui

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX
