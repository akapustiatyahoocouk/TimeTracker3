include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    Component.cpp \
    ComponentManager.cpp \
    FromString.cpp \
    Gpl3Licene.cpp \
    IComponent.cpp \
    ILicense.cpp \
    IMessageDigest.cpp \
    IPlugin.cpp \
    LicenseManager.cpp \
    MessageDigestManager.cpp \
    Mutex.cpp \
    PluginManager.cpp \
    Settings.cpp \
    Sha1MessageDigest.cpp \
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
    License.hpp \
    Linkage.hpp \
    Macros.hpp \
    MessageDigest.hpp \
    Plugin.hpp \
    Settings.hpp \
    Sync.hpp \
    ToString.hpp

RESOURCES += \
    tt3-util.qrc
