include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    AboutDialog.cpp \
    AskYesNoDialog.cpp \
    Component.cpp \
    CreateUserDialog.cpp \
    CurrentSkin.cpp \
    CurrentTheme.cpp \
    EditStringDialog.cpp \
    ErrorDialog.cpp \
    GeneralAppearancePreferences.cpp \
    GeneralAppearancePreferencesEditor.cpp \
    GeneralDialogsPreferences.cpp \
    GeneralDialogsPreferencesEditor.cpp \
    GeneralPreferences.cpp \
    GeneralStartupPreferences.cpp \
    GeneralStartupPreferencesEditor.cpp \
    LoginDialog.cpp \
    ModifyUserDialog.cpp \
    NewWorkspaceDialog.cpp \
    Preferences.cpp \
    PreferencesDialog.cpp \
    PreferencesEditor.cpp \
    PreferencesManager.cpp \
    SelectWorkspaceDialog.cpp \
    ShowConfigurationDialog.cpp \
    ShowLicenseDialog.cpp \
    SkinManager.cpp \
    StandardThemes.cpp \
    ThemeManager.cpp \
    UserManager.cpp

HEADERS += \
    API.hpp \
    AboutDialog.hpp \
    AskYesNoDialog.hpp \
    Components.hpp \
    CreateUserDialog.hpp \
    EditStringDialog.hpp \
    ErrorDialog.hpp \
    GeneralAppearancePreferencesEditor.hpp \
    GeneralDialogsPreferencesEditor.hpp \
    GeneralStartupPreferencesEditor.hpp \
    Linkage.hpp \
    LoginDialog.hpp \
    ModifyUserDialog.hpp \
    NewWorkspaceDialog.hpp \
    Preferences.hpp \
    PreferencesDialog.hpp \
    PreferencesEditor.hpp \
    SelectWorkspaceDialog.hpp \
    ShowConfigurationDialog.hpp \
    ShowLicenseDialog.hpp \
    Skin.hpp \
    Theme.hpp \
    UserManager.hpp

FORMS += \
    AboutDialog.ui \
    AskYesNoDialog.ui \
    CreateUserDialog.ui \
    EditStringDialog.ui \
    ErrorDialog.ui \
    GeneralAppearancePreferencesEditor.ui \
    GeneralDialogsPreferencesEditor.ui \
    GeneralStartupPreferencesEditor.ui \
    LoginDialog.ui \
    ModifyUserDialog.ui \
    NewWorkspaceDialog.ui \
    PreferencesDialog.ui \
    SelectWorkspaceDialog.ui \
    ShowConfigurationDialog.ui \
    ShowLicenseDialog.ui \
    UserManager.ui

RESOURCES += \
    tt3-gui.qrc

LIBS += -ltt3-ws -ltt3-db-api -ltt3-util
