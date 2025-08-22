include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    Component.cpp \
    Credentials.cpp \
    CurrentCredentials.cpp \
    CurrentWorkspace.cpp \
    Workspace.cpp \
    WorkspaceAddress.cpp \
    WorkspaceType.cpp \
    WorkspaceTypeManager.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    Credentials.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Notifications.hpp \
    Validator.hpp \
    Workspace.hpp \
    WorkspaceAddress.hpp \
    WorkspaceType.hpp

LIBS += -ltt3-db-api -ltt3-util
