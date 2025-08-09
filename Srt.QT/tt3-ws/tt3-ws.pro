include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    Component.cpp \
    WorkspaceAddress.cpp \
    WorkspaceType.cpp \
    WorkspaceTypeRegistry.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    Linkage.hpp \
    WorkspaceAddress.hpp \
    WorkspaceType.hpp

LIBS += -ltt3-db-api -ltt3-util
