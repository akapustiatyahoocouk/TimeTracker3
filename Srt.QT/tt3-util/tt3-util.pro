include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    Component.cpp \
    ComponentRegistry.cpp \
    FromString.cpp \
    Mutex.cpp \
    Plugin.cpp \
    PluginManager.cpp \
    Settings.cpp \
    TimeSpan.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    Collections.hpp \
    Component.hpp \
    Components.hpp \
    DateTime.hpp \
    Exceptions.hpp \
    FromString.hpp \
    Linkage.hpp \
    Macros.hpp \
    Plugin.hpp \
    Settings.hpp \
    Sync.hpp \
    ToString.hpp
