include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    AccountImpl.cpp \
    ActivityImpl.cpp \
    ActivityTypeImpl.cpp \
    Component.cpp \
    Credentials.cpp \
    Exceptions.cpp \
    ObjectImpl.cpp \
    PrincipalImpl.cpp \
    PublicActivityImpl.cpp \
    UserImpl.cpp \
    WorkspaceAddressImpl.cpp \
    WorkspaceImpl.cpp \
    WorkspaceTypeImpl.cpp \
    WorkspaceTypeManager.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Activity.hpp \
    ActivityType.hpp \
    Classes.hpp \
    Components.hpp \
    Credentials.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Notifications.hpp \
    Object.hpp \
    Principal.hpp \
    PublicActivity.hpp \
    User.hpp \
    Validator.hpp \
    Workload.hpp \
    Workspace.hpp \
    WorkspaceAddress.hpp \
    WorkspaceType.hpp

LIBS += -ltt3-db-api -ltt3-util
