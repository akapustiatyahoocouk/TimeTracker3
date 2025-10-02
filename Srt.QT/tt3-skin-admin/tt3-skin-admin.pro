include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_ADMIN_LIBRARY

SOURCES += \
    Component.cpp \
    MainFrame.cpp \
    Plugins.cpp \
    RecentWorkspaceOpener.cpp \
    Skin.cpp

HEADERS += \
    API.hpp \
    Classes.hpp \
    Components.hpp \
    Linkage.hpp \
    MainFrame.hpp \
    Skin.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    tt3-skin-admin.qrc

FORMS += \
    MainFrame.ui

LIBS += -ltt3-gui -ltt3-ws -ltt3-db-api -ltt3-util
