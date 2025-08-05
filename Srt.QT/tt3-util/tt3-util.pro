include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    ComponentRegistry.cpp \
    FromString.cpp \
    Plugin.cpp \
    PluginManager.cpp \
    Settings.cpp \
    ToString.cpp \
    UtilComponent.cpp \
    UtilSettings.cpp

HEADERS += \
    API.hpp \
    Collections.hpp \
    Component.hpp \
    Components.hpp \
    FromString.hpp \
    Linkage.hpp \
    Macros.hpp \
    Plugin.hpp \
    Settings.hpp \
    ToString.hpp
