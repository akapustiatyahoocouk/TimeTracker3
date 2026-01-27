include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_SQL_LIBRARY

SOURCES += \
    Account.cpp \
    Component.cpp \
    Database.cpp \
    Object.cpp \
    Principal.cpp \
    Statement.cpp \
    Transaction.cpp \
    User.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    CachedProperty.hpp \
    Classes.hpp \
    Component.hpp \
    Database.hpp \
    Linkage.hpp \
    Object.hpp \
    Principal.hpp \
    ResultSet.hpp \
    Statement.hpp \
    Transaction.hpp \
    User.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-sql.qrc
