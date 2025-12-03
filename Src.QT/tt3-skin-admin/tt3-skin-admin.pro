include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_ADMIN_LIBRARY

SOURCES += \
    Component.cpp \
    MainFrame.cpp \
    Skin.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Component.hpp \
    Linkage.hpp \
    MainFrame.hpp \
    Skin.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    tt3-skin-admin.qrc

FORMS += \
    MainFrame.ui

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX
