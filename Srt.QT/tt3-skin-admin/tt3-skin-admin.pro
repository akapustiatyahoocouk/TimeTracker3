include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_ADMIN_LIBRARY

SOURCES += \
    AdminSkin.cpp \
    AdminSkinMainFrame.cpp \
    Component.cpp \
    Plugins.cpp

HEADERS += \
    API.hpp \
    AdminSkin.hpp \
    AdminSkinMainFrame.hpp \
    Components.hpp \
    Linkage.hpp

RESOURCES += \
    tt3-skin-admin.qrc

FORMS += \
    AdminSkinMainFrame.ui

LIBS += -ltt3-gui -ltt3-util
