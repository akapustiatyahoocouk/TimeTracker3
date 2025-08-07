include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_API_LIBRARY

SOURCES += \
    Component.cpp \
    DatabaseTypeRegistry.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Exceptions.hpp \
    Linkage.hpp

LIBS += -ltt3-util
