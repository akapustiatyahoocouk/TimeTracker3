include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_DB_SQL_LIBRARY

SOURCES += \
    Account.cpp \
    Activity.cpp \
    ActivityType.cpp \
    Component.cpp \
    Database.cpp \
    Object.cpp \
    Principal.cpp \
    PrivateActivity.cpp \
    PrivateTask.cpp \
    PublicActivity.cpp \
    PublicTask.cpp \
    Statement.cpp \
    Task.cpp \
    Transaction.cpp \
    User.cpp \
    WorkStream.cpp \
    Workload.cpp

HEADERS += \
    API.hpp \
    Account.hpp \
    Activity.hpp \
    ActivityType.hpp \
    CachedProperty.hpp \
    Classes.hpp \
    Component.hpp \
    Database.hpp \
    Linkage.hpp \
    Object.hpp \
    Principal.hpp \
    PrivateActivity.hpp \
    PrivateTask.hpp \
    PublicActivity.hpp \
    PublicTask.hpp \
    ResultSet.hpp \
    Statement.hpp \
    Task.hpp \
    Transaction.hpp \
    User.hpp \
    WorkStream.hpp \
    Workload.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-db-sql.qrc
