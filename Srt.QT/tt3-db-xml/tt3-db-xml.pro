include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_XML_LIBRARY

SOURCES += \
    Account.cpp \
    Component.cpp \
    Database.cpp \
    DatabaseAddress.cpp \
    DatabaseType.cpp \
    Object.cpp \
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
    DatabaseType.hpp \
    Linkage.hpp \
    Object.hpp \
    Principal.hpp \
    User.hpp

LIBS += -ltt3-db-api -ltt3-util

RESOURCES += \
    tt3-db-xml.qrc
