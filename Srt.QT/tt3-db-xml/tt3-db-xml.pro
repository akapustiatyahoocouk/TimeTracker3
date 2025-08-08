include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_XML_LIBRARY

SOURCES += \
    Component.cpp \
    DatabaseAddress.cpp \
    DatabaseType.cpp \
    Plugins.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Linkage.hpp

LIBS += -ltt3-db-api -ltt3-util
