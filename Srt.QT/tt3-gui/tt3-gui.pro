include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    AboutDialog.cpp \
    ActivityTypeManager.cpp \
    AskYesNoDialog.cpp \
    ColorManager.cpp \
    Component.cpp \
    CreateAccountDialog.cpp \
    CreateActivityTypeDialog.cpp \
    CreatePublicActivityDialog.cpp \
    CreatePublicTaskDialog.cpp \
    CreateUserDialog.cpp \
    CurrentActivity.cpp \
    CurrentCredentials.cpp \
    CurrentSkin.cpp \
    CurrentTheme.cpp \
    CurrentWorkspace.cpp \
    DestroyAccountDialog.cpp \
    DestroyActivityTypeDialog.cpp \
    DestroyPublicActivityDialog.cpp \
    DestroyPublicTaskDialog.cpp \
    DestroyUserDialog.cpp \
    EditStringDialog.cpp \
    EnterActivityStartCommentDialog.cpp \
    EnterActivityStopCommentDialog.cpp \
    EnterTaskCompletionCommentDialog.cpp \
    ErrorDialog.cpp \
    GeneralAppearancePreferences.cpp \
    GeneralAppearancePreferencesEditor.cpp \
    GeneralDialogsPreferences.cpp \
    GeneralDialogsPreferencesEditor.cpp \
    GeneralPreferences.cpp \
    GeneralStartupPreferences.cpp \
    GeneralStartupPreferencesEditor.cpp \
    LabelDecorations.cpp \
    LoginDialog.cpp \
    ManageActivityTypesDialog.cpp \
    ManagePublicActivitiesDialog.cpp \
    ManagePublicTasksDialog.cpp \
    ManageUsersDialog.cpp \
    ModifyAccountDialog.cpp \
    ModifyActivityTypeDialog.cpp \
    ModifyPublicActivityDialog.cpp \
    ModifyPublicTaskDialog.cpp \
    ModifyUserDialog.cpp \
    NewWorkspaceDialog.cpp \
    Preferences.cpp \
    PreferencesDialog.cpp \
    PreferencesEditor.cpp \
    PreferencesManager.cpp \
    PrivateActivityManager.cpp \
    PublicActivityManager.cpp \
    PublicTaskManager.cpp \
    SelectPublicTaskParentDialog.cpp \
    SelectWorkspaceDialog.cpp \
    ShowConfigurationDialog.cpp \
    ShowLicenseDialog.cpp \
    SkinManager.cpp \
    StandardThemes.cpp \
    ThemeManager.cpp \
    TreeWidgetDecorations.cpp \
    UserManager.cpp

HEADERS += \
    API.hpp \
    AboutDialog.hpp \
    ActivityTypeManager.hpp \
    AskYesNoDialog.hpp \
    Color.hpp \
    Components.hpp \
    CreateAccountDialog.hpp \
    CreateActivityTypeDialog.hpp \
    CreatePublicActivityDialog.hpp \
    CreatePublicTaskDialog.hpp \
    CreateUserDialog.hpp \
    CurrentActivity.hpp \
    CurrentCredentials.hpp \
    CurrentWorkspace.hpp \
    DestroyAccountDialog.hpp \
    DestroyActivityTypeDialog.hpp \
    DestroyPublicActivityDialog.hpp \
    DestroyPublicTaskDialog.hpp \
    DestroyUserDialog.hpp \
    EditStringDialog.hpp \
    EnterActivityStartCommentDialog.hpp \
    EnterActivityStopCommentDialog.hpp \
    EnterTaskCompletionCommentDialog.hpp \
    ErrorDialog.hpp \
    GeneralAppearancePreferencesEditor.hpp \
    GeneralDialogsPreferencesEditor.hpp \
    GeneralStartupPreferencesEditor.hpp \
    Linkage.hpp \
    LoginDialog.hpp \
    ManageActivityTypesDialog.hpp \
    ManagePublicActivitiesDialog.hpp \
    ManagePublicTasksDialog.hpp \
    ManageUsersDialog.hpp \
    ModifyAccountDialog.hpp \
    ModifyActivityTypeDialog.hpp \
    ModifyPublicActivityDialog.hpp \
    ModifyPublicTaskDialog.hpp \
    ModifyUserDialog.hpp \
    NewWorkspaceDialog.hpp \
    Preferences.hpp \
    PreferencesDialog.hpp \
    PreferencesEditor.hpp \
    PrivateActivityManager.hpp \
    PublicActivityManager.hpp \
    PublicTaskManager.hpp \
    SelectPublicTaskParentDialog.hpp \
    SelectWorkspaceDialog.hpp \
    ShowConfigurationDialog.hpp \
    ShowLicenseDialog.hpp \
    Skin.hpp \
    Theme.hpp \
    UiHelpers.hpp \
    UserManager.hpp \
    WidgetDecorations.hpp

FORMS += \
    AboutDialog.ui \
    ActivityTypeManager.ui \
    AskYesNoDialog.ui \
    CreateAccountDialog.ui \
    CreateActivityTypeDialog.ui \
    CreatePublicActivityDialog.ui \
    CreatePublicTaskDialog.ui \
    CreateUserDialog.ui \
    EditStringDialog.ui \
    ErrorDialog.ui \
    GeneralAppearancePreferencesEditor.ui \
    GeneralDialogsPreferencesEditor.ui \
    GeneralStartupPreferencesEditor.ui \
    LoginDialog.ui \
    ManageActivityTypesDialog.ui \
    ManagePublicActivitiesDialog.ui \
    ManagePublicTasksDialog.ui \
    ManageUsersDialog.ui \
    ModifyAccountDialog.ui \
    ModifyActivityTypeDialog.ui \
    ModifyPublicActivityDialog.ui \
    ModifyPublicTaskDialog.ui \
    ModifyUserDialog.ui \
    NewWorkspaceDialog.ui \
    PreferencesDialog.ui \
    PrivateActivityManager.ui \
    PublicActivityManager.ui \
    PublicTaskManager.ui \
    SelectPublicTaskParentDialog.ui \
    SelectWorkspaceDialog.ui \
    ShowConfigurationDialog.ui \
    ShowLicenseDialog.ui \
    UserManager.ui

RESOURCES += \
    tt3-gui.qrc

LIBS += -ltt3-ws -ltt3-db-api -ltt3-util
