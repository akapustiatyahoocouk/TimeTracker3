include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_SQLITE3_LIBRARY

SOURCES += \
    Component.cpp

HEADERS += \
    API.hpp \
    Component.hpp \
    Libs/sqlite3.h \
    Libs/sqlite3ext.h \
    Linkage.hpp

LIBS += \
    -ltt3-db-sql$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-sqlite3.qrc
