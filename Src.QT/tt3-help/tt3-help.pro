include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_HELP_LIBRARY

SOURCES += \
    Component.cpp \
    LocalFileContentLoader.cpp \
    LocalSiteHelpLoader.cpp \
    SimpleHelpCollection.cpp \
    SimpleHelpTopic.cpp \
    SimpleHelpTopicCollection.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    ContentLoader.hpp \
    ContentType.hpp \
    Exceptions.hpp \
    Help.hpp \
    Linkage.hpp \
    LocalSiteHelpLoader.hpp \
    SimpleHelp.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-help.qrc
