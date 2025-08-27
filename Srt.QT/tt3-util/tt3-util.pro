include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    Component.cpp \
    ComponentManager.cpp \
    FromString.cpp \
    IComponent.cpp \
    ILicense.cpp \
    IMessageDigest.cpp \
    IPlugin.cpp \
    ISubsystem.cpp \
    Identifiers.cpp \
    LicenseManager.cpp \
    MessageDigestManager.cpp \
    Mutex.cpp \
    PluginManager.cpp \
    Settings.cpp \
    Sha1MessageDigest.cpp \
    StandardLicenses.cpp \
    StandardSubsystems.cpp \
    SubsystemManager.cpp \
    TimeSpan.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    BlockingQueue.hpp \
    Component.hpp \
    Components.hpp \
    DateTime.hpp \
    Exceptions.hpp \
    FromString.hpp \
    Identifiers.hpp \
    License.hpp \
    Linkage.hpp \
    Macros.hpp \
    MessageDigest.hpp \
    Plugin.hpp \
    Settings.hpp \
    Subsystem.hpp \
    Sync.hpp \
    ToString.hpp

RESOURCES += \
    tt3-util.qrc
