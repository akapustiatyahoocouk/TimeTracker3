//
//  tt3-skin-admin/MainFrame.cpp - tt3::skin::admin::MainFrame class implementation
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
#include "tt3-skin-admin/API.hpp"
#include "ui_MainFrame.h"
using namespace tt3::skin::admin;

//////////
//  Construction/destruction
MainFrame::MainFrame(QWidget * parent)
    :   QMainWindow(parent),
        _ui(new Ui::MainFrame),
        _savePositionTimer(this),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    _labelDecorations = tt3::gui::LabelDecorations(_ui->currentActivityLabel);

    _loadPosition();
    _updateMruWorkspaces();

    //  Create custom controls
    _manageUsersTabLayout = new QStackedLayout();
    _userManager = new tt3::gui::UserManager(_ui->manageUsersTab);
    _manageUsersTabLayout->addWidget(_userManager);
    _ui->manageUsersTab->setLayout(_manageUsersTabLayout);

    _manageActivityTypesTabLayout = new QStackedLayout();
    _activityTypeManager = new tt3::gui::ActivityTypeManager(_ui->manageActivityTypesTab);
    _manageActivityTypesTabLayout->addWidget(_activityTypeManager);
    _ui->manageActivityTypesTab->setLayout(_manageActivityTypesTabLayout);

    _managePublicActivitiesTabLayout = new QStackedLayout();
    _publicActivityManager = new tt3::gui::PublicActivityManager(_ui->managePublicActivitiesTab);
    _managePublicActivitiesTabLayout->addWidget(_publicActivityManager);
    _ui->managePublicActivitiesTab->setLayout(_managePublicActivitiesTabLayout);

    _managePublicTasksTabLayout = new QStackedLayout();
    _publicTaskManager = new tt3::gui::PublicTaskManager(_ui->managePublicTasksTab);
    _managePublicTasksTabLayout->addWidget(_publicTaskManager);
    _ui->managePublicTasksTab->setLayout(_managePublicTasksTabLayout);

    _managePrivateActivitiesTabLayout = new QStackedLayout();
    _privateActivityManager = new tt3::gui::PrivateActivityManager(_ui->managePrivateActivitiesTab);
    _managePrivateActivitiesTabLayout->addWidget(_privateActivityManager);
    _ui->managePrivateActivitiesTab->setLayout(_managePrivateActivitiesTabLayout);

    _managePrivateTasksTabLayout = new QStackedLayout();
    _privateTaskManager = new tt3::gui::PrivateTaskManager(_ui->managePrivateTasksTab);
    _managePrivateTasksTabLayout->addWidget(_privateTaskManager);
    _ui->managePrivateTasksTab->setLayout(_managePrivateTasksTabLayout);

    _manageProjectsTabLayout = new QStackedLayout();
    _projectManager = new tt3::gui::ProjectManager(_ui->manageProjectsTab);
    _manageProjectsTabLayout->addWidget(_projectManager);
    _ui->manageProjectsTab->setLayout(_manageProjectsTabLayout);

    _manageWorkStreamsTabLayout = new QStackedLayout();
    _workStreamManager = new tt3::gui::WorkStreamManager(_ui->manageWorkStreamsTab);
    _manageWorkStreamsTabLayout->addWidget(_workStreamManager);
    _ui->manageWorkStreamsTab->setLayout(_manageWorkStreamsTabLayout);

    _manageBeneficiariesTabLayout = new QStackedLayout();
    _beneficiaryManager = new tt3::gui::BeneficiaryManager(_ui->manageBeneficiariesTab);
    _manageBeneficiariesTabLayout->addWidget(_beneficiaryManager);
    _ui->manageBeneficiariesTab->setLayout(_manageBeneficiariesTabLayout);

    //  Set up signal handlers
    _savePositionTimer.setSingleShot(true);
    connect(&_savePositionTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_savePositionTimerTimeout);

    connect(&tt3::gui::theCurrentWorkspace,
            &tt3::gui::CurrentWorkspace::changed,
            this,
            &MainFrame::_currentWorkspaceChanged,
            Qt::ConnectionType::QueuedConnection);
    connect(&tt3::gui::theCurrentCredentials,
            &tt3::gui::CurrentCredentials::changed,
            this,
            &MainFrame::_currentCredentialsChanged,
            Qt::ConnectionType::QueuedConnection);
    connect(&tt3::gui::theCurrentActivity,
            &tt3::gui::CurrentActivity::changed,
            this,
            &MainFrame::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);
    connect(&tt3::gui::theCurrentTheme,
            &tt3::gui::CurrentTheme::changed,
            this,
            &MainFrame::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Done
    _ui->managersTabWidget->setCurrentIndex(Component::Settings::instance()->mainFrameCurrentTab);
    _trackPosition = true;
    refresh();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

MainFrame::~MainFrame()
{
    _refreshTimer.stop();
    delete _ui;

    for (RecentWorkspaceOpener * o : _recentWorkspaceOpeners)
    {
        delete o;
    }
}

//////////
//  QWidget
void MainFrame::moveEvent(QMoveEvent * event)
{
    QMainWindow::moveEvent(event);
    if (_trackPosition)
    {
        _savePositionTimer.start(500);
    }
}

void MainFrame::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    if (_trackPosition)
    {
        _savePositionTimer.start(500);
    }
}

void MainFrame::closeEvent(QCloseEvent * event)
{
    event->ignore();
    _onActionExit();
}

//////////
//  Operations
void MainFrame::refresh()
{
    tt3::ws::Workspace workspace = tt3::gui::theCurrentWorkspace;
    tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
    //  Frame title
    QString title = "TimeTracker3";
    if (credentials.isValid())
    {
        title += " [";
        title += credentials.login();
        title += "]";
    }
    if (workspace != nullptr)
    {
        title += " - ";
        title += workspace->address()->displayForm();
        if (workspace->isReadOnly())
        {
            title += " [read-only]";
        }
    }
    this->setWindowTitle(title);

    //  Menu items
    _ui->actionCloseWorkspace->setEnabled(workspace != nullptr);
    _ui->menuEdit->setEnabled(workspace != nullptr);
    _ui->actionManageUsers->setEnabled(workspace != nullptr);
    _ui->actionManageActivityTypes->setEnabled(workspace != nullptr);
    _ui->actionManagePublicActivities->setEnabled(workspace != nullptr);
    _ui->actionManagePublicTasks->setEnabled(workspace != nullptr);
    _ui->actionManagePrivateActivities->setEnabled(workspace != nullptr);
    _ui->actionManagePrivateTasks->setEnabled(workspace != nullptr);
    _ui->actionManageProjects->setEnabled(workspace != nullptr);
    _ui->actionManageWorkStreams->setEnabled(workspace != nullptr);
    _ui->actionManageBeneficiaries->setEnabled(workspace != nullptr);
    _ui->actionRefresh->setEnabled(workspace != nullptr);

    //  Controls
    _userManager->refresh();
    _activityTypeManager->refresh();
    _publicActivityManager->refresh();
    _publicTaskManager->refresh();
    _privateActivityManager->refresh();
    _privateTaskManager->refresh();
    _projectManager->refresh();
    _workStreamManager->refresh();
    _beneficiaryManager->refresh();

    _refreshCurrentActivityControls();
}

//////////
//  Implementation
void MainFrame::_loadPosition()
{
    this->setGeometry(Component::Settings::instance()->mainFrameBounds);
    if (Component::Settings::instance()->mainFrameMaximized)
    {
        this->showMaximized();
    }
}

void MainFrame::_savePosition()
{
    if (this->isMaximized())
    {
        Component::Settings::instance()->mainFrameMaximized = true;
    }
    else if (!this->isMinimized())
    {
        Component::Settings::instance()->mainFrameBounds = this->geometry();
        Component::Settings::instance()->mainFrameMaximized = false;
    }
}

bool MainFrame::_createWorkspace(
    tt3::ws::WorkspaceAddress workspaceAddress,
    const QString & adminUser,
    const QString & adminLogin, const QString & adminPassword)
{
    Q_ASSERT(workspaceAddress != nullptr);

    //  If the workspaceAddress refers to the currently
    //  open workspace, the call is an error
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {   //  OOPS!
        tt3::gui::ErrorDialog::show(
            this,
            tt3::db::api::AlreadyExistsException(
                "Workspace", "location", workspaceAddress->displayForm()));
        return false;
    }

    //  Create & use
    try
    {
        tt3::ws::Workspace workspace
        {
            workspaceAddress->workspaceType()->createWorkspace(
                workspaceAddress, adminUser, adminLogin, adminPassword)
        };
        //  If there is a "current activity", record & stop it
        try
        {
            tt3::gui::theCurrentActivity.replaceWith(nullptr);
        }
        catch (const tt3::util::Exception & ex)
        {
            tt3::gui::ErrorDialog::show(this, ex);
        }
        //  Replace the "current" workspace
        tt3::gui::theCurrentWorkspace.swap(workspace);
        tt3::ws::Component::Settings::instance()->addRecentWorkspace(workspaceAddress);
        _updateMruWorkspaces();
        //  The previously "current" workspace is closed
        //  when replaced
        if (workspace != nullptr)
        {
            try
            {
                workspace->close();
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Close errors, however, don't stop us ay
                //  this stage - a new workspace has already been
                //  created & selected as "current"
                tt3::gui::ErrorDialog::show(this, ex);
                return false;
            }
        }
        //  We need to change th "current" credentials to allow access to the new workspace
        tt3::gui::theCurrentCredentials = tt3::ws::Credentials(adminLogin, adminPassword);
        tt3::gui::Component::Settings::instance()->lastLogin = adminLogin;  //  ...for next startup
        //  Done
        refresh();
        return true;
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
        return false;
    }
}

bool MainFrame::_openWorkspace(
        tt3::ws::WorkspaceAddress workspaceAddress,
        tt3::ws::OpenMode openMode
    )
{
    Q_ASSERT(workspaceAddress != nullptr);

    //  If the workspaceAddress refers to the currently
    //  open workspace, we don't need to re-open
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {
        return true;
    }

    //  Open & use
    try
    {
        tt3::ws::Workspace workspace
            { workspaceAddress->workspaceType()->openWorkspace(workspaceAddress, openMode) };
        //  If the current credentials do not allow access
        //  to the newly open workspace, what do we do?
        if (!_reconcileCurrntCredentials(workspace))
        {   //  No reconciliation - stop opening the workspace
            workspace->close();
            return false;
        }
        //  If there is a "current activity", record & stop it
        try
        {
            tt3::gui::theCurrentActivity.replaceWith(nullptr);
        }
        catch (const tt3::util::Exception & ex)
        {
            tt3::gui::ErrorDialog::show(this, ex);
        }
        //  Use the newly open workspace
        tt3::gui::theCurrentWorkspace.swap(workspace);
        tt3::ws::Component::Settings::instance()->addRecentWorkspace(workspaceAddress);
        _updateMruWorkspaces();
        //  The previously "current" workspace is closed
        //  when replaced
        if (workspace != nullptr)
        {
            try
            {
                workspace->close();
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Close errors, however, don't stop us ay
                //  this stage - a workspace has already been
                //  opened & selected as "current"
                tt3::gui::ErrorDialog::show(this, ex);
                return false;
            }
        }
        //  Done
        refresh();
        return true;
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
        refresh();
        return false;
    }
}

bool MainFrame::_reconcileCurrntCredentials(const tt3::ws::Workspace & workspace)
{
    Q_ASSERT(workspace != nullptr && workspace->isOpen());

    if (workspace->canAccess(tt3::gui::theCurrentCredentials))
    {   //  Already OK
        return true;
    }
    //  Ask the user whether to login with different credentials
    for (; ; )
    {
        tt3::gui::ChooseReloginDialog dlg(this, workspace->address());
        if (dlg.doModal() != tt3::gui::ChooseReloginDialog::Result::Yes)
        {   //  OOPS! The user does NOT want to re-login
            return false;
        }
        //  The user DOES choose to re-login
        tt3::gui::LoginDialog dlg1(this, "");
        if (dlg1.doModal() != tt3::gui::LoginDialog::Result::Ok)
        {   //  The user has cancelled the re-login
            return false;
        }
        if (workspace->canAccess(dlg1.credentials()))
        {   //  Access OK
            tt3::gui::theCurrentCredentials = dlg1.credentials();
            return true;
        }
    }
}

void MainFrame::_destroyWorkspace(tt3::ws::WorkspaceAddress workspaceAddress)
{
    Q_ASSERT(workspaceAddress != nullptr);

    //  If the workspaceAddress refers to the currently
    //  open workspace, we can't destroy it
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {
        tt3::gui::ErrorDialog::show(
            this,
            "Cannot destroy workspace\n" +
                workspaceAddress->displayForm() +
                "\n because it is currently in use");
        return;
    }

    //  Do the work
    try
    {
        workspaceAddress->workspaceType()->destroyWorkspace(tt3::gui::theCurrentCredentials, workspaceAddress);
        //  No need to refreh()!
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void MainFrame::_updateMruWorkspaces()
{
    QMenu * menu = _ui->actionRecentWorkspaces->menu();
    if (menu == nullptr)
    {
        menu = new QMenu();
        _ui->actionRecentWorkspaces->setMenu(menu);
    }
    menu->clear();
    for (RecentWorkspaceOpener * o : _recentWorkspaceOpeners)
    {
        delete o;
    }
    _recentWorkspaceOpeners.clear();

    tt3::ws::WorkspaceAddressesList mru =
        tt3::ws::Component::Settings::instance()->recentWorkspaces;
    for (int i = 0; i < mru.size() && i < 9; i++)
    {
        tt3::ws::WorkspaceAddress a = mru[i];
        QAction * action = menu->addAction(
            a->workspaceType()->smallIcon(),
            "&" + QString(QChar('1' + i)) + " - " + a->displayForm());
        //  Handle "action triggered" signal
        RecentWorkspaceOpener * opener = new RecentWorkspaceOpener(this, a);
        _recentWorkspaceOpeners.append(opener);
        connect(action,
                &QAction::triggered,
                opener,
                &RecentWorkspaceOpener::_onTriggered);
    }
    _ui->actionRecentWorkspaces->setEnabled(!menu->isEmpty());
}

void MainFrame::_refreshCurrentActivityControls()
{
    QPalette currentActivityLabelPalette = _ui->currentActivityLabel->palette();
    if (tt3::gui::theCurrentActivity != nullptr)
    {
        qint64 secs = qMax(0, tt3::gui::theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
        char s[32];
        sprintf(s, " [%d:%02d:%02d]",
                int(secs / (60 * 60)),
                int((secs / 60) % 60),
                int(secs % 60));

        _ui->currentActivityLabel->setAutoFillBackground(true);
        currentActivityLabelPalette.setColor(QPalette::Window, _labelDecorations.liveStatusBackground);
        currentActivityLabelPalette.setColor(QPalette::WindowText, _labelDecorations.foreground);

        try
        {
            _ui->currentActivityLabel->setText(tt3::gui::theCurrentActivity->displayName(tt3::gui::theCurrentCredentials) + s);    //  may throw
            _ui->currentActivityLabel->setFont(_labelDecorations.emphasisFont);
            _ui->stopActivityPushButton->setEnabled(true);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex.errorMessage();
            currentActivityLabelPalette.setColor(QPalette::Window, _labelDecorations.errorBackground);
            currentActivityLabelPalette.setColor(QPalette::WindowText, _labelDecorations.errorForeground);
            _ui->currentActivityLabel->setText(ex.errorMessage());
            _ui->currentActivityLabel->setFont(_labelDecorations.emphasisFont);
        }
    }
    else
    {
        _ui->currentActivityLabel->setAutoFillBackground(false);
        currentActivityLabelPalette.setColor(QPalette::Window, _labelDecorations.background);
        currentActivityLabelPalette.setColor(QPalette::WindowText, _labelDecorations.disabledForeground);

        _ui->currentActivityLabel->setText("There is no current activity");
        _ui->currentActivityLabel->setFont(_labelDecorations.font);
        _ui->stopActivityPushButton->setEnabled(false);
    }
    _ui->currentActivityLabel->setPalette(currentActivityLabelPalette);
}

//////////
//  Signal handlers
void MainFrame::_savePositionTimerTimeout()
{
    _savePosition();
}

void MainFrame::_onActionNewWorkspace()
{
    tt3::gui::NewWorkspaceDialog dlg(this);
    if (dlg.doModal() == tt3::gui::NewWorkspaceDialog::Result::Ok)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        Q_ASSERT(workspaceAddress != nullptr);
        _createWorkspace(workspaceAddress, dlg.adminUser(), dlg.adminLogin(), dlg.adminPassword());
    }
}

void MainFrame::_onActionOpenWorkspace()
{
    tt3::gui::SelectWorkspaceDialog dlg(
        this,
        tt3::gui::SelectWorkspaceDialog::OptionalControls::OpenModeSelection);
    if (dlg.doModal() == tt3::gui::SelectWorkspaceDialog::Result::Ok)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        tt3::ws::OpenMode openMode = dlg.selectedOpenMode();
        Q_ASSERT(workspaceAddress != nullptr);
        _openWorkspace(workspaceAddress, openMode);
    }
}

void MainFrame::_onActionCloseWorkspace()
{
    //  Get the "current workspace: out of the way
    tt3::ws::Workspace workspace = nullptr;
    tt3::gui::theCurrentWorkspace.swap(workspace);
    //  So?
    if (workspace == nullptr)
    {   //  ...there wasn't one - there's nothing to close
        return;
    }
    //  Confirm...
    if (workspace != nullptr &&
        tt3::gui::Component::Settings::instance()->confirmCloseWorkspace)
    {
        tt3::gui::ConfirmCloseWorkspaceDialog dlg(this, workspace->address());
        if (dlg.doModal() != tt3::gui::ConfirmCloseWorkspaceDialog::Result::Yes)
        {   //  ...and the user has said "no" - restore the "current"
            //  database as it was, and we're done
            tt3::gui::theCurrentWorkspace.swap(workspace);
            return;
        }
    }
    //  Either the user has confirmed the close, or
    //  the confirmation was implicit - either way, close!
    //  ...and close
    try
    {
        workspace->close();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! close() error - report, but stay "closed"
        tt3::gui::ErrorDialog::show(this, ex);
    }
    refresh();
}

void MainFrame::_onActionDestroyWorkspace()
{
    tt3::gui::SelectWorkspaceDialog dlg(
        this,
        tt3::gui::SelectWorkspaceDialog::OptionalControls::None);
    if (dlg.doModal() == tt3::gui::SelectWorkspaceDialog::Result::Ok)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        Q_ASSERT(workspaceAddress != nullptr);
        //  Always confirm!
        tt3::gui::ConfirmDestroyWorkspaceDialog dlg(this, workspaceAddress);
        if (dlg.doModal() == tt3::gui::ConfirmDestroyWorkspaceDialog::Result::Yes)
        {   //  Do it!
            _destroyWorkspace(workspaceAddress);
        }
    }
}

void MainFrame::_onActionRestart()
{
    //  Confirm...
    if (tt3::gui::Component::Settings::instance()->confirmRestart)
    {
        tt3::gui::ConfirmRestartDialog dlg(this);
        if (dlg.doModal() != tt3::gui::ConfirmRestartDialog::Result::Yes)
        {
            return;
        }
    }
    //  ...and restart
    QApplication::exit(-1);
}

void MainFrame::_onActionExit()
{   //  Confirm...
    if (tt3::gui::Component::Settings::instance()->confirmExit)
    {
        tt3::gui::ConfirmExitDialog dlg(this);
        if (dlg.doModal() != tt3::gui::ConfirmExitDialog::Result::Yes)
        {
            return;
        }
    }
    //  ...and exit
    QApplication::exit(0);
}

void MainFrame::_onActionManageUsers()
{   //  TODO switch to "Users" tab instead
    tt3::gui::ManageUsersDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageActivityTypes()
{   //  TODO switch to "Activity types" tab instead
    tt3::gui::ManageActivityTypesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePublicActivities()
{   //  TODO switch to "Public activities" tab instead
    tt3::gui::ManagePublicActivitiesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePublicTasks()
{   //  TODO switch to "Public tasks" tab instead
    tt3::gui::ManagePublicTasksDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePrivateActivities()
{   //  TODO switch to "Private activities" tab instead
    tt3::gui::ManagePrivateActivitiesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePrivateTasks()
{   //  TODO switch to "Private tasks" tab instead
    tt3::gui::ManagePrivateTasksDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageProjects()
{   //  TODO switch to "Projects" tab instead
    tt3::gui::ManageProjectsDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageWorkStreams()
{   //  TODO switch to "WorkStreams" tab instead
    tt3::gui::ManageWorkStreamsDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageBeneficiaries()
{   //  TODO switch to "Beneficiaries" tab instead
    tt3::gui::ManageBeneficiariesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionRefresh()
{
    if (tt3::gui::theCurrentWorkspace != nullptr)
    {
        try
        {
            tt3::gui::theCurrentWorkspace->refresh();
        }
        catch (const tt3::util::Exception & ex)
        {
            tt3::gui::ErrorDialog::show(this, ex);
        }
    }
    refresh();
}

void MainFrame::_onActionPreferences()
{
    tt3::gui::PreferencesDialog dlg(this);
    dlg.doModal();
}

void MainFrame::_onActionLoginAsDifferentUser()
{
    tt3::gui::LoginDialog dlg(this, "");
    if (dlg.doModal() == tt3::gui::LoginDialog::Result::Ok)
    {
        tt3::ws::Credentials credentials = dlg.credentials();
        try
        {
            if (tt3::gui::theCurrentWorkspace == nullptr ||
                tt3::gui::theCurrentWorkspace->canAccess(credentials))   //  may throw
            {   //  Login is fine
                tt3::gui::theCurrentCredentials = credentials;
            }
            else
            {   //  OOPS! Do we close the current workspace? Or ignore login attempt?
                tt3::gui::ConfirmDropWorkspaceDialog dlg1(this, tt3::gui::theCurrentWorkspace->address());
                if (dlg1.doModal() == tt3::gui::ConfirmDropWorkspaceDialog::Result::Yes)
                {   //  Yes - close the current workspace and keep the new credentials
                    tt3::ws::Workspace workspace = nullptr;
                    tt3::gui::theCurrentWorkspace.swap(workspace);
                    workspace->close(); //  may throw
                    tt3::gui::theCurrentCredentials = credentials;
                }
                //  else forget about the re-login and keep the workspace open
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            tt3::gui::ErrorDialog::show(this, ex);
        }
        refresh();
    }
}

void MainFrame::_onActionHelpContent()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void MainFrame::_onActionHelpIndex()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void MainFrame::_onActionHelpSearch()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void MainFrame::_onActionAbout()
{
    tt3::gui::AboutDialog dlg(this);
    dlg.doModal();
}

void MainFrame::_stopActivityPushButtonClicked()
{
    try
    {
        tt3::gui::theCurrentActivity.replaceWith(nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
    refresh();
}

void MainFrame::_workspaceClosed(tt3::ws::WorkspaceClosedNotification)
{
    refresh();
}

void MainFrame::_currentWorkspaceChanged(tt3::ws::Workspace, tt3::ws::Workspace)
{
    _userManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _activityTypeManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _publicActivityManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _publicTaskManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _privateActivityManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _privateTaskManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _projectManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _workStreamManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _beneficiaryManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    refresh();
}

void MainFrame::_currentCredentialsChanged(tt3::ws::Credentials, tt3::ws::Credentials)
{
    _userManager->setCredentials(tt3::gui::theCurrentCredentials);
    _activityTypeManager->setCredentials(tt3::gui::theCurrentCredentials);
    _publicActivityManager->setCredentials(tt3::gui::theCurrentCredentials);
    _publicTaskManager->setCredentials(tt3::gui::theCurrentCredentials);
    _privateActivityManager->setCredentials(tt3::gui::theCurrentCredentials);
    _privateTaskManager->setCredentials(tt3::gui::theCurrentCredentials);
    _projectManager->setCredentials(tt3::gui::theCurrentCredentials);
    _workStreamManager->setCredentials(tt3::gui::theCurrentCredentials);
    _beneficiaryManager->setCredentials(tt3::gui::theCurrentCredentials);
    refresh();
}

void MainFrame::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    _refreshCurrentActivityControls();
}

void MainFrame::_currentThemeChanged(tt3::gui::ITheme*, tt3::gui::ITheme*)
{
    _labelDecorations = tt3::gui::LabelDecorations(_ui->currentActivityLabel);
    refresh();
}

void MainFrame::_managersTabWidgetCurrentChanged(int)
{
    if (_trackPosition)
    {   //  i.e. constructor has finished
        refresh();
        Component::Settings::instance()->mainFrameCurrentTab = _ui->managersTabWidget->currentIndex();
    }
}

void MainFrame::_refreshTimerTimeout()
{
    if (tt3::gui::theCurrentActivity != nullptr)
    {
        _refreshCurrentActivityControls();
    }
}

//  End of tt3-skin-admin/MainFrame.cpp
