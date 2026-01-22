include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_SQL_LIBRARY

SOURCES += \
    Component.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Component.hpp \
    Linkage.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-sql.qrc
