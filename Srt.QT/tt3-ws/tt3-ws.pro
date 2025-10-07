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
    PrivateActivityImpl.cpp \
    PrivateTaskImpl.cpp \
    ProjectImpl.cpp \
    PublicActivityImpl.cpp \
    PublicTaskImpl.cpp \
    TaskImpl.cpp \
    UserImpl.cpp \
    WorkStreamImpl.cpp \
    WorkloadImpl.cpp \
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
    PrivateActivity.hpp \
    PrivateTask.hpp \
    Project.hpp \
    PublicActivity.hpp \
    PublicTask.hpp \
    Task.hpp \
    User.hpp \
    Validator.hpp \
    WorkStream.hpp \
    Workload.hpp \
    Workspace.hpp \
    WorkspaceAddress.hpp \
    WorkspaceType.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += -ltt3-db-api -ltt3-util
