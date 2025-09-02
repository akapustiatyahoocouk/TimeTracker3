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
        _ui(new Ui::MainFrame)
{
    _ui->setupUi(this);

    _loadPosition();
    _updateMruWorkspaces();

    //  Create custom controls
    _manageUsersTabLayout = new QStackedLayout();
    _userManager = new tt3::gui::UserManager(_ui->manageUsersTab);
    _manageUsersTabLayout->addWidget(_userManager);
    _ui->manageUsersTab->setLayout(_manageUsersTabLayout);

    //  Set up signal handlers
    _savePositionTimer.setSingleShot(true);
    connect(&_savePositionTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_savePositionTimerTimeout);

    connect(&tt3::ws::theCurrentWorkspace,
            &tt3::ws::CurrentWorkspace::changed,
            this,
            &MainFrame::_onCurrentWorkspaceChanged);
    connect(&tt3::ws::theCurrentCredentials,
            &tt3::ws::CurrentCredentials::changed,
            this,
            &MainFrame::_onCurrentCredentialsChanged);

    //  Done
    _trackPosition = true;
    refresh();
}

MainFrame::~MainFrame()
{
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
    event->accept();
    _onActionExit();
}

//////////
//  Operations
void MainFrame::refresh()
{
    tt3::ws::Workspace currentWorkspace = tt3::ws::theCurrentWorkspace;
    //  Frame title
    QString title = "TimeTracker3";
    if (tt3::ws::theCurrentCredentials.isValid())
    {
        title += " [";
        title += tt3::ws::theCurrentCredentials.login();
        title += "]";
    }
    if (currentWorkspace != nullptr)
    {
        title += " - ";
        title += currentWorkspace->address()->displayForm();
    }
    this->setWindowTitle(title);

    //  Menu items
    _ui->actionCloseWorkspace->setEnabled(currentWorkspace != nullptr);

    //  Controls
    _userManager->refresh();
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
    if (tt3::ws::theCurrentWorkspace != nullptr &&
        tt3::ws::theCurrentWorkspace->address() == workspaceAddress)
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
        //  TODO if there is a "current activity", record & stop it
        tt3::ws::theCurrentWorkspace.swap(workspace);
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
        tt3::ws::theCurrentCredentials = tt3::ws::Credentials(adminLogin, adminPassword);
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

bool MainFrame::_openWorkspace(tt3::ws::WorkspaceAddress workspaceAddress)
{
    Q_ASSERT(workspaceAddress != nullptr);

    //  If the workspaceAddress refers to the currently
    //  open workspace, we don't need to re-open
    if (tt3::ws::theCurrentWorkspace != nullptr &&
        tt3::ws::theCurrentWorkspace->address() == workspaceAddress)
    {
        return true;
    }

    //  Open & use
    try
    {
        tt3::ws::Workspace workspace
            { workspaceAddress->workspaceType()->openWorkspace(workspaceAddress) };
        //  If the current credentials do not allow access
        //  to the newly open workspace, what do we do?
        if (!_reconcileCurrntCredentials(workspace))
        {   //  No reconciliation - stop opening the workspace
            return false;
        }
        //  TODO if there is a "current activity", record & stop it
        //  Use the newly open workspace
        tt3::ws::theCurrentWorkspace.swap(workspace);
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

    if (workspace->canAccess(tt3::ws::theCurrentCredentials))
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
            tt3::ws::theCurrentCredentials = dlg1.credentials();
            return true;
        }
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
    tt3::gui::SelectWorkspaceDialog dlg(this);
    if (dlg.doModal() == tt3::gui::SelectWorkspaceDialog::Result::Ok)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        Q_ASSERT(workspaceAddress != nullptr);
        _openWorkspace(workspaceAddress);
    }
}

void MainFrame::_onActionCloseWorkspace()
{
    //  Get the "current workspace: out of the way
    tt3::ws::Workspace workspace = nullptr;
    tt3::ws::theCurrentWorkspace.swap(workspace);
    //  So?
    if (workspace == nullptr)
    {   //  ..there wasn't one - there's nothing to close
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
            tt3::ws::theCurrentWorkspace.swap(workspace);
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
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
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

void MainFrame::_onActionPreferences()
{
    tt3::gui::PreferencesDialog dlg(this);
    dlg.doModal();
}

void MainFrame::_onActionLoginAsDifferentUser()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
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

void MainFrame::_onWorkspaceClosed(tt3::ws::WorkspaceClosedNotification)
{
    refresh();
}

void MainFrame::_onCurrentWorkspaceChanged(tt3::ws::Workspace /*before*/, tt3::ws::Workspace /*after*/)
{
    _userManager->setWorkspace(tt3::ws::theCurrentWorkspace);
    refresh();
}

void MainFrame::_onCurrentCredentialsChanged(tt3::ws::Credentials /*before*/, tt3::ws::Credentials /*after*/)
{
    _userManager->setCredentials(tt3::ws::theCurrentCredentials);
    refresh();
}

//  End of tt3-skin-admin/MainFrame.cpp
