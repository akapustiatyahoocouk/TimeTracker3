include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_SKIN_ADMIN_LIBRARY

SOURCES += \
    AdminSkin.cpp \
    Component.cpp \
    MainFrame.cpp \
    Plugins.cpp

HEADERS += \
    API.hpp \
    AdminSkin.hpp \
    Components.hpp \
    Linkage.hpp \
    MainFrame.hpp

RESOURCES += \
    tt3-skin-admin.qrc

FORMS += \
    MainFrame.ui

LIBS += -ltt3-gui -ltt3-ws -ltt3-util
