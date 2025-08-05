include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_ADMIN_LIBRARY

SOURCES += \
    AdminSkin.cpp \
    AdminSkinComponent.cpp \
    AdminSkinPlugins.cpp \
    AdminSkinSettings.cpp

HEADERS += \
    API.hpp \
    AdminSkin.hpp \
    Components.hpp \
    Linkage.hpp

LIBS += -ltt3-gui -ltt3-util

RESOURCES += \
    tt3-skin-admin.qrc
