include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_XML_LIBRARY

SOURCES += \
    Account.cpp \
    Activity.cpp \
    ActivityType.cpp \
    Component.cpp \
    Database.cpp \
    DatabaseAddress.cpp \
    DatabaseType.cpp \
    Object.cpp \
    Plugins.cpp \
    Principal.cpp \
    PrivateActivity.cpp \
    PublicActivity.cpp \
    PublicTask.cpp \
    Task.cpp \
    User.cpp \
    Workload.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Activity.hpp \
    ActivityType.hpp \
    Classes.hpp \
    Components.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Linkage.hpp \
    Object.hpp \
    Principal.hpp \
    PrivateActivity.hpp \
    PublicActivity.hpp \
    PublicTask.hpp \
    Task.hpp \
    User.hpp \
    Workload.hpp

LIBS += -ltt3-db-api -ltt3-util

RESOURCES += \
    tt3-db-xml.qrc
