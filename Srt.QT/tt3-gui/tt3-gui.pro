include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    AboutDialog.cpp \
    AskYesNoDialog.cpp \
    Component.cpp \
    CurrentSkin.cpp \
    ErrorDialog.cpp \
    GeneralAppearancePreferences.cpp \
    GeneralAppearancePreferencesEditor.cpp \
    GeneralPreferences.cpp \
    GeneralStartupPreferences.cpp \
    GeneralStartupPreferencesEditor.cpp \
    LoginDialog.cpp \
    NewWorkspaceDialog.cpp \
    Preferences.cpp \
    PreferencesDialog.cpp \
    PreferencesEditor.cpp \
    PreferencesManager.cpp \
    SelectWorkspaceDialog.cpp \
    ShowConfigurationDialog.cpp \
    ShowLicenseDialog.cpp \
    SkinManager.cpp \
    UserManager.cpp

HEADERS += \
    API.hpp \
    AboutDialog.hpp \
    AskYesNoDialog.hpp \
    Components.hpp \
    ErrorDialog.hpp \
    GeneralAppearancePreferencesEditor.hpp \
    GeneralStartupPreferencesEditor.hpp \
    Linkage.hpp \
    LoginDialog.hpp \
    NewWorkspaceDialog.hpp \
    Preferences.hpp \
    PreferencesDialog.hpp \
    PreferencesEditor.hpp \
    SelectWorkspaceDialog.hpp \
    ShowConfigurationDialog.hpp \
    ShowLicenseDialog.hpp \
    Skin.hpp \
    UserManager.hpp

FORMS += \
    AboutDialog.ui \
    GeneralAppearancePreferencesEditor.ui \
    GeneralStartupPreferencesEditor.ui \
    LoginDialog.ui \
    NewWorkspaceDialog.ui \
    PreferencesDialog.ui \
    SelectWorkspaceDialog.ui \
    ShowConfigurationDialog.ui \
    ShowLicenseDialog.ui \
    UserManager.ui

RESOURCES += \
    tt3-gui.qrc

LIBS += -ltt3-ws -ltt3-util
