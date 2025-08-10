include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    Component.cpp \
    Workspace.cpp \
    WorkspaceAddress.cpp \
    WorkspaceType.cpp \
    WorkspaceTypeRegistry.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Workspace.hpp \
    WorkspaceAddress.hpp \
    WorkspaceType.hpp

LIBS += -ltt3-db-api -ltt3-util
