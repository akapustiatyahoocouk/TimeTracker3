include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_HELP_CLIENT_LIBRARY

SOURCES += \
    Component.cpp \
    HelpClient.cpp

HEADERS += \
    API.hpp \
    Components.hpp \
    HelpClient.hpp \
    Linkage.hpp

LIBS += \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-help-client.qrc
