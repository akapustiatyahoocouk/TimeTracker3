include(../tt3.pri)
QT += widgets-private

TEMPLATE = lib
DEFINES += TT3_HELP_LIBRARY

SOURCES += \
    Component.cpp \
    ContentLoaderFactory.cpp \
    Exceptions.cpp \
    HelpSiteBuilder.cpp \
    LocalFileContentLoader.cpp \
    LocalSiteHelpLoader.cpp \
    Serializer.cpp \
    SimpleHelpCollection.cpp \
    SimpleHelpTopic.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Component.hpp \
    ContentLoader.hpp \
    Exceptions.hpp \
    Help.hpp \
    HelpSiteBuilder.hpp \
    Linkage.hpp \
    LocalSiteHelpLoader.hpp \
    Serialization.hpp \
    SimpleHelp.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-help.qrc
