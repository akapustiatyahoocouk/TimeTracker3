include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_SQLITE3_LIBRARY

SOURCES += \
    Component.cpp \
    Database.cpp \
    DatabaseAddress.cpp \
    DatabaseType.cpp \
    SQLite3.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Component.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Libs/sqlite3.h \
    Libs/sqlite3ext.h \
    Linkage.hpp \
    SQLite3.hpp

LIBS += \
    -ltt3-db-sql$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-sqlite3.qrc

#   Third-party libs
win32 {
    QMAKE_POST_LINK += copy /y $$shell_path($${PWD}/Libs/sqlite3.dll) $$shell_path($${DESTDIR}/) $$escape_expand(\\n\\t)
    QMAKE_CLEAN += $$shell_path($${DESTDIR}/sqlite3.dll)
}
