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

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-help$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

DISTFILES += \
    tt3.rc

