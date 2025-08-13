include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_API_LIBRARY

SOURCES += \
    Component.cpp \
    Database.cpp \
    DatabaseObjectTypes.cpp \
    DatabaseTypeRegistry.cpp \
    DefaultValidator.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Classes.hpp \
    Components.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseObject.hpp \
    DatabaseObjectType.hpp \
    DatabaseType.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Principal.hpp \
    User.hpp \
    Validator.hpp

LIBS += -ltt3-util

RESOURCES += \
    tt3-db-api.qrc
