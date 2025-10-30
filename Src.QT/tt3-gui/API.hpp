//
//  tt3-gui/API.hpp - tt3-gui master header
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#pragma once

//////////
//  Dependencies
#include "tt3-ws/API.hpp"
#include "tt3-util/API.hpp"

//////////
//  tt3-gui components
#include "tt3-gui/Linkage.hpp"
#include "tt3-gui/Components.hpp"

//  Miscellaneous helpers
#include "tt3-gui/Exceptions.hpp"
#include "tt3-gui/Skin.hpp"
#include "tt3-gui/Theme.hpp"
#include "tt3-gui/Preferences.hpp"
#include "tt3-gui/Color.hpp"
#include "tt3-gui/CurrentCredentials.hpp"
#include "tt3-gui/CurrentWorkspace.hpp"
#include "tt3-gui/CurrentActivity.hpp"
#include "tt3-gui/UiHelpers.hpp"
#include "tt3-gui/QuickReport.hpp"

//  Controls
#include "tt3-gui/WidgetDecorations.hpp"
#include "tt3-gui/PreferencesEditor.hpp"
#include "tt3-gui/GeneralAppearancePreferencesEditor.hpp"
#include "tt3-gui/GeneralStartupPreferencesEditor.hpp"
#include "tt3-gui/GeneralDialogsPreferencesEditor.hpp"
#include "tt3-gui/QuickReportView.hpp"
#include "tt3-gui/DailyWorkQuickReportView.hpp"

#include "tt3-gui/UserManager.hpp"
#include "tt3-gui/ActivityTypeManager.hpp"
#include "tt3-gui/PublicActivityManager.hpp"
#include "tt3-gui/PublicTaskManager.hpp"
#include "tt3-gui/PrivateActivityManager.hpp"
#include "tt3-gui/PrivateTaskManager.hpp"
#include "tt3-gui/ProjectManager.hpp"
#include "tt3-gui/WorkStreamManager.hpp"
#include "tt3-gui/BeneficiaryManager.hpp"
#include "tt3-gui/MyDayManager.hpp"
#include "tt3-gui/QuickReportBrowser.hpp"

//  Dialogs
#include "tt3-gui/MessageDialog.hpp"
#include "tt3-gui/ErrorDialog.hpp"
#include "tt3-gui/AskYesNoDialog.hpp"
#include "tt3-gui/EditStringDialog.hpp"
#include "tt3-gui/NewWorkspaceDialog.hpp"
#include "tt3-gui/SelectWorkspaceDialog.hpp"
#include "tt3-gui/LoginDialog.hpp"
#include "tt3-gui/AboutDialog.hpp"
#include "tt3-gui/PreferencesDialog.hpp"
#include "tt3-gui/ShowLicenseDialog.hpp"
#include "tt3-gui/ShowConfigurationDialog.hpp"
#include "tt3-gui/CreateUserDialog.hpp"
#include "tt3-gui/ModifyUserDialog.hpp"
#include "tt3-gui/DestroyUserDialog.hpp"
#include "tt3-gui/CreateAccountDialog.hpp"
#include "tt3-gui/ModifyAccountDialog.hpp"
#include "tt3-gui/DestroyAccountDialog.hpp"
#include "tt3-gui/CreateActivityTypeDialog.hpp"
#include "tt3-gui/ModifyActivityTypeDialog.hpp"
#include "tt3-gui/DestroyActivityTypeDialog.hpp"
#include "tt3-gui/CreatePublicActivityDialog.hpp"
#include "tt3-gui/ModifyPublicActivityDialog.hpp"
#include "tt3-gui/DestroyPublicActivityDialog.hpp"
#include "tt3-gui/CreatePublicTaskDialog.hpp"
#include "tt3-gui/ModifyPublicTaskDialog.hpp"
#include "tt3-gui/DestroyPublicTaskDialog.hpp"
#include "tt3-gui/SelectPublicTaskParentDialog.hpp"
#include "tt3-gui/CreatePrivateActivityDialog.hpp"
#include "tt3-gui/ModifyPrivateActivityDialog.hpp"
#include "tt3-gui/DestroyPrivateActivityDialog.hpp"
#include "tt3-gui/CreatePrivateTaskDialog.hpp"
#include "tt3-gui/ModifyPrivateTaskDialog.hpp"
#include "tt3-gui/DestroyPrivateTaskDialog.hpp"
#include "tt3-gui/SelectPrivateTaskParentDialog.hpp"
#include "tt3-gui/CreateProjectDialog.hpp"
#include "tt3-gui/ModifyProjectDialog.hpp"
#include "tt3-gui/DestroyProjectDialog.hpp"
#include "tt3-gui/SelectProjectParentDialog.hpp"
#include "tt3-gui/CreateWorkStreamDialog.hpp"
#include "tt3-gui/ModifyWorkStreamDialog.hpp"
#include "tt3-gui/DestroyWorkStreamDialog.hpp"
#include "tt3-gui/CreateBeneficiaryDialog.hpp"
#include "tt3-gui/ModifyBeneficiaryDialog.hpp"
#include "tt3-gui/DestroyBeneficiaryDialog.hpp"
#include "tt3-gui/DestroyWorkDialog.hpp"
#include "tt3-gui/DestroyEventDialog.hpp"
#include "tt3-gui/SelectWorkloadDialog.hpp"
#include "tt3-gui/SelectWorkloadsDialog.hpp"
#include "tt3-gui/SelectBeneficiariesDialog.hpp"

#include "tt3-gui/ConfirmForgetWorkspaceDialog.hpp"
#include "tt3-gui/ConfirmCloseWorkspaceDialog.hpp"
#include "tt3-gui/ConfirmDropWorkspaceDialog.hpp"
#include "tt3-gui/ConfirmDestroyWorkspaceDialog.hpp"
#include "tt3-gui/ConfirmRestartDialog.hpp"
#include "tt3-gui/ConfirmExitDialog.hpp"
#include "tt3-gui/ChooseReloginDialog.hpp"
#include "tt3-gui/ConfirmResetPageSettingsDialog.hpp"
#include "tt3-gui/ConfirmResetAllSettingsDialog.hpp"
#include "tt3-gui/RestartRequiredDialog.hpp"
#include "tt3-gui/AddEmailAddressDialog.hpp"
#include "tt3-gui/ModifyEmailAddressDialog.hpp"
#include "tt3-gui/EnterActivityStartCommentDialog.hpp"
#include "tt3-gui/EnterActivityStopCommentDialog.hpp"
#include "tt3-gui/EnterTaskCompletionCommentDialog.hpp"
#include "tt3-gui/EnterCommentDialog.hpp"

#include "tt3-gui/ManageUsersDialog.hpp"
#include "tt3-gui/ManageActivityTypesDialog.hpp"
#include "tt3-gui/ManagePublicActivitiesDialog.hpp"
#include "tt3-gui/ManagePublicTasksDialog.hpp"
#include "tt3-gui/ManagePrivateActivitiesDialog.hpp"
#include "tt3-gui/ManagePrivateTasksDialog.hpp"
#include "tt3-gui/ManageProjectsDialog.hpp"
#include "tt3-gui/ManageWorkStreamsDialog.hpp"
#include "tt3-gui/ManageBeneficiariesDialog.hpp"
#include "tt3-gui/ManageMyDayDialog.hpp"
#include "tt3-gui/QuickReportsDialog.hpp"

#include "tt3-gui/ManageQuickPicksListDialog.hpp"

#include "tt3-gui/HelpClient.hpp"
#include "tt3-gui/HelpSiteBuilder.hpp"
#include "tt3-gui/HelpBuilderProgressWindow.hpp"

//  End of tt3-gui/API.hpp
