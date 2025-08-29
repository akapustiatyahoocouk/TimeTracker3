include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    AccountImpl.cpp \
    Component.cpp \
    Credentials.cpp \
    CurrentCredentials.cpp \
    CurrentWorkspace.cpp \
    Exceptions.cpp \
    ObjectImpl.cpp \
    PrincipalImpl.cpp \
    UserImpl.cpp \
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
    Workspace.hpp \
    WorkspaceAddressImpl.hpp \
    WorkspaceTypeImpl.hpp

LIBS += -ltt3-db-api -ltt3-util
