include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_TOOLS_BACKUP_LIBRARY

SOURCES += \
    BackupProgressDialog.cpp \
    BackupTool.cpp \
    BackupWriter.cpp \
    Component.cpp \
    ConfigureBackupDialog.cpp \
    Plugin.cpp

HEADERS += \
    API.hpp \
    BackupProgressDialog.hpp \
    BackupTool.hpp \
    BackupWriter.hpp \
    Component.hpp \
    ConfigureBackupDialog.hpp \
    Linkage.hpp \
    Plugin.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    tt3-tools-backup.qrc

FORMS += \
    BackupProgressDialog.ui \
    ConfigureBackupDialog.ui

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX
