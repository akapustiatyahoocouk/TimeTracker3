include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_XML_LIBRARY

SOURCES += \
    Account.cpp \
    Activity.cpp \
    ActivityType.cpp \
    Beneficiary.cpp \
    Component.cpp \
    Database.cpp \
    DatabaseAddress.cpp \
    DatabaseLock.cpp \
    DatabaseType.cpp \
    Event.cpp \
    Object.cpp \
    Preferences.cpp \
    PreferencesEditor.cpp \
    Principal.cpp \
    PrivateActivity.cpp \
    PrivateTask.cpp \
    Project.cpp \
    PublicActivity.cpp \
    PublicTask.cpp \
    Task.cpp \
    User.cpp \
    Work.cpp \
    WorkStream.cpp \
    Workload.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Activity.hpp \
    ActivityType.hpp \
    Beneficiary.hpp \
    Classes.hpp \
    Component.hpp \
    Database.hpp \
    DatabaseAddress.hpp \
    DatabaseLock.hpp \
    DatabaseType.hpp \
    Event.hpp \
    Linkage.hpp \
    Object.hpp \
    Preferences.hpp \
    PreferencesEditor.hpp \
    Principal.hpp \
    PrivateActivity.hpp \
    PrivateTask.hpp \
    Project.hpp \
    PublicActivity.hpp \
    PublicTask.hpp \
    Task.hpp \
    User.hpp \
    Work.hpp \
    WorkStream.hpp \
    Workload.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-xml.qrc

FORMS += \
    PreferencesEditor.ui
