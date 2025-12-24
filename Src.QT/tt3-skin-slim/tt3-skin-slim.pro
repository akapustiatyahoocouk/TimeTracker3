include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_SLIM_LIBRARY

SOURCES += \
    Component.cpp \
    MainFrame.cpp \
    Skin.cpp

HEADERS += \
    API.hpp \
    Component.hpp \
    Linkage.hpp \
    MainFrame.hpp \
    Skin.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-report$$TARGET_SUFFIX \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-skin-slim.qrc

FORMS += \
    MainFrame.ui
