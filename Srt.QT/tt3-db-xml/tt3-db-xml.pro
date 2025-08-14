include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_XML_LIBRARY

SOURCES += \
    Component.cpp \
    Database.cpp \
    DatabaseAddress.cpp \
    DatabaseObject.cpp \
    DatabaseType.cpp \
    Plugins.cpp \
    Principal.cpp \
    User.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Classes.hpp \
    Components.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseObject.hpp \
    DatabaseType.hpp \
    Linkage.hpp \
    Principal.hpp \
    User.hpp

LIBS += -ltt3-db-api -ltt3-util

RESOURCES += \
    tt3-db-xml.qrc
