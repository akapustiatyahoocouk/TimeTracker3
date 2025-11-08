include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    AccountImpl.cpp \
    ActivityImpl.cpp \
    ActivityTypeImpl.cpp \
    BackupCredentials.cpp \
    BeneficiaryImpl.cpp \
    Component.cpp \
    Credentials.cpp \
    EventImpl.cpp \
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
    WorkImpl.cpp \
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
    Beneficiary.hpp \
    Classes.hpp \
    Components.hpp \
    Credentials.hpp \
    Event.hpp \
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
    Work.hpp \
    WorkStream.hpp \
    Workload.hpp \
    Workspace.hpp \
    WorkspaceAddress.hpp \
    WorkspaceType.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-ws.qrc
