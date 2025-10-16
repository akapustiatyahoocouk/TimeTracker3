include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_API_LIBRARY

SOURCES += \
    Capabilities.cpp \
    ChangeNotifier.cpp \
    Component.cpp \
    DatabaseTypeManager.cpp \
    DefaultValidator.cpp \
    Exceptions.cpp \
    ObjectTypes.cpp \
    Oid.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Activity.hpp \
    ActivityType.hpp \
    Beneficiary.hpp \
    Capabilities.hpp \
    Classes.hpp \
    Components.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseType.hpp \
    Event.hpp \
    Exceptions.hpp \
    Linkage.hpp \
    Notifications.hpp \
    Object.hpp \
    ObjectType.hpp \
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
    Workload.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-api.qrc
