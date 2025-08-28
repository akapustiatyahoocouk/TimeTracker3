include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    Component.cpp \
    Credentials.cpp \
    CurrentCredentials.cpp \
    CurrentWorkspace.cpp \
    Exceptions.cpp \
    WorkspaceAddressImpl.cpp \
    WorkspaceImpl.cpp \
    WorkspaceTypeImpl.cpp \
    WorkspaceTypeManager.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Classes.hpp \
    Components.hpp \
    Credentials.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Notifications.hpp \
    Object.hpp \
    Principal.hpp \
    User.hpp \
    Validator.hpp \
    WorkspaceAddressImpl.hpp \
    WorkspaceImpl.hpp \
    WorkspaceTypeImpl.hpp

LIBS += -ltt3-db-api -ltt3-util
