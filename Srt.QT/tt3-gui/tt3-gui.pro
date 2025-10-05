include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    AboutDialog.cpp \
    ActivityTypeManager.cpp \
    AddEmailAddressDialog.cpp \
    AskYesNoDialog.cpp \
    ColorManager.cpp \
    Component.cpp \
    CreateAccountDialog.cpp \
    CreateActivityTypeDialog.cpp \
    CreatePrivateActivityDialog.cpp \
    CreatePrivateTaskDialog.cpp \
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
    DestroyPrivateActivityDialog.cpp \
    DestroyPrivateTaskDialog.cpp \
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
    ManagePrivateActivitiesDialog.cpp \
    ManagePrivateTasksDialog.cpp \
    ManagePublicActivitiesDialog.cpp \
    ManagePublicTasksDialog.cpp \
    ManageUsersDialog.cpp \
    ModifyAccountDialog.cpp \
    ModifyActivityTypeDialog.cpp \
    ModifyEmailAddressDialog.cpp \
    ModifyPrivateActivityDialog.cpp \
    ModifyPrivateTaskDialog.cpp \
    ModifyPublicActivityDialog.cpp \
    ModifyPublicTaskDialog.cpp \
    ModifyUserDialog.cpp \
    NewWorkspaceDialog.cpp \
    Preferences.cpp \
    PreferencesDialog.cpp \
    PreferencesEditor.cpp \
    PreferencesManager.cpp \
    PrivateActivityManager.cpp \
    PrivateTaskManager.cpp \
    PublicActivityManager.cpp \
    PublicTaskManager.cpp \
    SelectPrivateTaskParentDialog.cpp \
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
    AddEmailAddressDialog.hpp \
    AskYesNoDialog.hpp \
    Color.hpp \
    Components.hpp \
    CreateAccountDialog.hpp \
    CreateActivityTypeDialog.hpp \
    CreatePrivateActivityDialog.hpp \
    CreatePrivateTaskDialog.hpp \
    CreatePublicActivityDialog.hpp \
    CreatePublicTaskDialog.hpp \
    CreateUserDialog.hpp \
    CurrentActivity.hpp \
    CurrentCredentials.hpp \
    CurrentWorkspace.hpp \
    DestroyAccountDialog.hpp \
    DestroyActivityTypeDialog.hpp \
    DestroyPrivateActivityDialog.hpp \
    DestroyPrivateTaskDialog.hpp \
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
    ManagePrivateActivitiesDialog.hpp \
    ManagePrivateTasksDialog.hpp \
    ManagePublicActivitiesDialog.hpp \
    ManagePublicTasksDialog.hpp \
    ManageUsersDialog.hpp \
    ModifyAccountDialog.hpp \
    ModifyActivityTypeDialog.hpp \
    ModifyEmailAddressDialog.hpp \
    ModifyPrivateActivityDialog.hpp \
    ModifyPrivateTaskDialog.hpp \
    ModifyPublicActivityDialog.hpp \
    ModifyPublicTaskDialog.hpp \
    ModifyUserDialog.hpp \
    NewWorkspaceDialog.hpp \
    Preferences.hpp \
    PreferencesDialog.hpp \
    PreferencesEditor.hpp \
    PrivateActivityManager.hpp \
    PrivateTaskManager.hpp \
    PublicActivityManager.hpp \
    PublicTaskManager.hpp \
    SelectPrivateTaskParentDialog.hpp \
    SelectPublicTaskParentDialog.hpp \
    SelectWorkspaceDialog.hpp \
    ShowConfigurationDialog.hpp \
    ShowLicenseDialog.hpp \
    Skin.hpp \
    Theme.hpp \
    UiHelpers.hpp \
    UserManager.hpp \
    WidgetDecorations.hpp

PRECOMPILED_HEADER = API.hpp

FORMS += \
    AboutDialog.ui \
    ActivityTypeManager.ui \
    AskYesNoDialog.ui \
    CreateAccountDialog.ui \
    CreateActivityTypeDialog.ui \
    CreatePrivateActivityDialog.ui \
    CreatePrivateTaskDialog.ui \
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
    ManagePrivateActivitiesDialog.ui \
    ManagePrivateTasksDialog.ui \
    ManagePublicActivitiesDialog.ui \
    ManagePublicTasksDialog.ui \
    ManageUsersDialog.ui \
    ModifyAccountDialog.ui \
    ModifyActivityTypeDialog.ui \
    ModifyPrivateActivityDialog.ui \
    ModifyPrivateTaskDialog.ui \
    ModifyPublicActivityDialog.ui \
    ModifyPublicTaskDialog.ui \
    ModifyUserDialog.ui \
    NewWorkspaceDialog.ui \
    PreferencesDialog.ui \
    PrivateActivityManager.ui \
    PrivateTaskManager.ui \
    PublicActivityManager.ui \
    PublicTaskManager.ui \
    SelectPrivateTaskParentDialog.ui \
    SelectPublicTaskParentDialog.ui \
    SelectWorkspaceDialog.ui \
    ShowConfigurationDialog.ui \
    ShowLicenseDialog.ui \
    UserManager.ui

RESOURCES += \
    tt3-gui.qrc

LIBS += -ltt3-ws -ltt3-db-api -ltt3-util
