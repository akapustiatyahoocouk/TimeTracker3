include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    Component.cpp \
    ComponentManager.cpp \
    FromString.cpp \
    MessageDigest.cpp \
    Mutex.cpp \
    Plugin.cpp \
    PluginManager.cpp \
    Settings.cpp \
    Sha1MessageDigest.cpp \
    TimeSpan.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    BlockingQueue.hpp \
    Component.hpp \
    Components.hpp \
    DateTime.hpp \
    Exceptions.hpp \
    FromString.hpp \
    Linkage.hpp \
    Macros.hpp \
    MessageDigest.hpp \
    Plugin.hpp \
    Settings.hpp \
    Sync.hpp \
    ToString.hpp

RESOURCES += \
    tt3-util.qrc
