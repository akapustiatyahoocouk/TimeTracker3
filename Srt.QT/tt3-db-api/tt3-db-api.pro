include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_API_LIBRARY

SOURCES += \
    Component.cpp \
    DatabaseTypeRegistry.cpp \
    DefaultValidator.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Validator.hpp

LIBS += -ltt3-util
