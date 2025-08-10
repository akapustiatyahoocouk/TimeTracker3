include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    Component.cpp \
    CurrentSkin.cpp \
    NewWorkspaceDialog.cpp \
    OpenWorkspaceDialog.cpp \
    SkinRegistry.cpp

HEADERS += \
    API.hpp \
    Components.hpp \
    Linkage.hpp \
    NewWorkspaceDialog.hpp \
    OpenWorkspaceDialog.hpp \
    Skin.hpp

LIBS += -ltt3-util

FORMS += \
    NewWorkspaceDialog.ui \
    OpenWorkspaceDialog.ui

RESOURCES += \
    tt3-gui.qrc
