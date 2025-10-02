include(../tt3.pri)

SOURCES += \
    Application.cpp \
    Component.cpp \
    Main.cpp

HEADERS += \
    API.hpp \
    Application.hpp \
    Components.hpp

PRECOMPILED_HEADER = API.hpp

FORMS += \

RESOURCES += \
    tt3.qrc

RC_FILE = tt3.rc

LIBS += -ltt3-gui -ltt3-ws -ltt3-db-api -ltt3-util

DISTFILES += \
    tt3.rc
