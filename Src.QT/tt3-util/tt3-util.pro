include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    Component.cpp \
    ComponentManager.cpp \
    CurrentLocale.cpp \
    Exceptions.cpp \
    FileResourceFactory.cpp \
    FromString.cpp \
    IComponent.cpp \
    ILicense.cpp \
    IMessageDigest.cpp \
    IResourceFactory.cpp \
    ISubsystem.cpp \
    Identifiers.cpp \
    LicenseManager.cpp \
    LocaleManager.cpp \
    MessageDigestManager.cpp \
    Mutex.cpp \
    ProductInformation.cpp \
    ResourceReader.cpp \
    Settings.cpp \
    StandardLicenses.cpp \
    StandardMessageDigests.Sha1.cpp \
    StandardSubsystems.cpp \
    SubsystemManager.cpp \
    TimeSpan.cpp \
    ToString.cpp \
    ToolManager.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Collections.hpp \
    Component.hpp \
    DateTime.hpp \
    Exceptions.hpp \
    FromString.hpp \
    Identifiers.hpp \
    License.hpp \
    Linkage.hpp \
    Locale.hpp \
    Macros.hpp \
    MessageDigest.hpp \
    ProductInformation.hpp \
    ResourceFactory.hpp \
    Settings.hpp \
    Subsystem.hpp \
    Sync.hpp \
    ToString.hpp \
    Tool.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    tt3-util.qrc
