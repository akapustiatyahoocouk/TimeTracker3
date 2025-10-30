include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_HELP_LIBRARY

SOURCES += \
    Component.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    Linkage.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-help.qrc
