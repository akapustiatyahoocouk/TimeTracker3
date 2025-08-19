include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    AboutDialog.cpp \
    Component.cpp \
    CurrentSkin.cpp \
    ErrorDialog.cpp \
    GeneralPreferences.cpp \
    LoginDialog.cpp \
    NewWorkspaceDialog.cpp \
    PreferencedEditor.cpp \
    Preferences.cpp \
    PreferencesRegistry.cpp \
    SelectWorkspaceDialog.cpp \
    SkinRegistry.cpp

HEADERS += \
    API.hpp \
    AboutDialog.hpp \
    Components.hpp \
    ErrorDialog.hpp \
    GeneralPreferences.hpp \
    Linkage.hpp \
    LoginDialog.hpp \
    NewWorkspaceDialog.hpp \
    Preferences.hpp \
    PreferencesEditor.hpp \
    SelectWorkspaceDialog.hpp \
    Skin.hpp

FORMS += \
    AboutDialog.ui \
    LoginDialog.ui \
    NewWorkspaceDialog.ui \
    SelectWorkspaceDialog.ui

RESOURCES += \
    tt3-gui.qrc

LIBS += -ltt3-ws -ltt3-util
