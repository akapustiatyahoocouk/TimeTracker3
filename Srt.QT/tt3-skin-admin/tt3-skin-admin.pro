include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_ADMIN_LIBRARY

SOURCES += \
    AdminSkin.cpp \
    AdminSkinComponent.cpp \
    AdminSkinMainFrame.cpp \
    AdminSkinPlugins.cpp \
    AdminSkinSettings.cpp

HEADERS += \
    API.hpp \
    AdminSkin.hpp \
    AdminSkinMainFrame.hpp \
    Components.hpp \
    Linkage.hpp

LIBS += -ltt3-gui -ltt3-util

RESOURCES += \
    tt3-skin-admin.qrc

FORMS += \
    AdminSkinMainFrame.ui
