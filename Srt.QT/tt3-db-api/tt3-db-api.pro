include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_API_LIBRARY

SOURCES += \
    Capabilities.cpp \
    ChangeNotifier.cpp \
    Component.cpp \
    Database.cpp \
    DatabaseTypeManager.cpp \
    DefaultValidator.cpp \
    ObjectTypes.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Capabilities.hpp \
    Classes.hpp \
    Components.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Notifications.hpp \
    Object.hpp \
    ObjectType.hpp \
    Principal.hpp \
    User.hpp \
    Validator.hpp

LIBS += -ltt3-util

RESOURCES += \
    tt3-db-api.qrc
