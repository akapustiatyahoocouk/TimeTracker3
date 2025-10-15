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
    -ltt3-gui-$$TT3_VERSION \
    -ltt3-ws-$$TT3_VERSION \
    -ltt3-db-api-$$TT3_VERSION \
    -ltt3-util-$$TT3_VERSION

DISTFILES += \
    tt3.rc
