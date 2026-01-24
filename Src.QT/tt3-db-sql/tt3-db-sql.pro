include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_SQL_LIBRARY

SOURCES += \
    Component.cpp \
    Database.cpp \
    Statement.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Component.hpp \
    Database.hpp \
    Linkage.hpp \
    Object.hpp \
    ResultSet.hpp \
    Statement.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-sql.qrc
