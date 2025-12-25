//
//  tt3-skin-slim/MainFrame.cpp - tt3::skin::slim::MainFrame class implementation
//  TODO Localize via Resources
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
#include "tt3-skin-slim/API.hpp"
#include "ui_MainFrame.h"
using namespace tt3::skin::slim;

#define TR(s)   s   /*  TODO kill off when done localizing */

//////////
//  Construction/destruction
MainFrame::MainFrame()
    :   QMainWindow(nullptr),
        _ui(new Ui::MainFrame),
        _trackPositionTimer(this),
        _savePositionTimer(this),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    //  TODO ? flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowMinMaxButtonsHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    this->setMinimumSize(MinimumSize);
    _loadPosition();

    //  Set up signal handlers
    _trackPositionTimer.setSingleShot(true);
    _savePositionTimer.setSingleShot(true);
    connect(&_trackPositionTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_trackPositionTimerTimeout);
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

    //  Theme change means widget decorations change
    connect(&tt3::gui::theCurrentTheme,
            &tt3::gui::CurrentTheme::changed,
            this,
            &MainFrame::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &MainFrame::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Done
    refresh();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &MainFrame::_refreshTimerTimeout);
    _refreshTimer.start(1000);

    //  TODO finish the implementation
}

MainFrame::~MainFrame()
{
    delete _ui;
}

//////////
//  QWidget
void MainFrame::moveEvent(QMoveEvent * event)
{
    QMainWindow::moveEvent(event);
    _savePositionTimer.start(500);
}

void MainFrame::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    _savePositionTimer.start(500);
}

void MainFrame::closeEvent(QCloseEvent * event)
{
    event->ignore();
    _onActionExit();
}

void MainFrame::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        _dragging = true;
        _dragAnchor = event->pos();
    }
}

void MainFrame::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    _dragging = false;
}

void MainFrame::mouseMoveEvent(QMouseEvent * event)
{
    if (_dragging)
    {
        int dx = event->pos().x() - _dragAnchor.x();
        int dy = event->pos().y() - _dragAnchor.x();
        QRect r = this->frameGeometry();
        this->move(r.x() + dx, r.y() + dy);
    }
}

void MainFrame::contextMenuEvent(QContextMenuEvent * event)
{
    _contextMenu.reset(_createContextMenu());
    _contextMenu->popup(event->globalPos());
}

void MainFrame::keyPressEvent(QKeyEvent * event)
{
    struct Shortcut
    {
        int key;                            //  Qt::Key_?
        Qt::KeyboardModifiers   modifiers;  //  Qt::?Modifier
        void (MainFrame::*handler)();
    };
    static const Shortcut shortcuts[] =
    {
        { Qt::Key_M, Qt::ControlModifier, &MainFrame::_onActionMinimize },
        { Qt::Key_N, Qt::ControlModifier, &MainFrame::_onActionNewWorkspace },
        { Qt::Key_O, Qt::ControlModifier, &MainFrame::_onActionOpenWorkspace },
        { Qt::Key_X, Qt::ControlModifier, &MainFrame::_onActionExit },
        { Qt::Key_P, Qt::ControlModifier, &MainFrame::_onActionPreferences }
    };

    for (const auto shortcut : shortcuts)
    {
        if (event->key() == shortcut.key &&
            event->modifiers() == shortcut.modifiers)
        {
            event->accept();    //  we're handling it!
            (this->*shortcut.handler)();
            return;
        }
    }
    /*  TODO
    if (event->key() == Qt::Key_M &&
        event->modifiers() == Qt::ControlModifier)
    {
        event->accept();    //  we're handling it!
        _onActionMinimize();
    }
    if (event->key() == Qt::Key_N &&
        event->modifiers() == Qt::ControlModifier)
    {
        event->accept();    //  we're handling it!
        _onActionNewWorkspace();
    }
    else if (event->key() == Qt::Key_X &&
             event->modifiers() == Qt::ControlModifier)
    {
        event->accept();    //  we're handling it!
        _onActionExit();
    }
    else if (event->key() == Qt::Key_P &&
             event->modifiers() == Qt::ControlModifier)
    {
        event->accept();    //  we're handling it!
        _onActionPreferences();
    }
    */
}

//////////
//  Operations
void MainFrame::show()
{
    if (!this->isVisible())
    {
        QMainWindow::show();
        //  Under X11 it will be some time before the window manager
        //  stabilizes the frame position; it is at THAT time that
        //  frame position needs to be loaded and tracjed
        _trackPositionTimer.start(500);
    }
    //  TODO if hidden, restore from system tray
}

void MainFrame::hide()
{
    if (this->isVisible())
    {
        _trackPositionTimer.stop();
        _trackPosition = false;
        QMainWindow::hide();
    }
}

void MainFrame::refresh()
{
    //  TODO
}

//////////
//  Implementation
void MainFrame::_loadPosition()
{
    _setFrameGeometry(Component::Settings::instance()->mainFrameBounds);
    _ensureWithinScreenBounds();
    if (Component::Settings::instance()->mainFrameMaximized)
    {
        this->showMaximized();
    }
}

void MainFrame::_savePosition()
{
    if (_trackPosition)
    {
        if (this->isMaximized())
        {
            Component::Settings::instance()->mainFrameMaximized = true;
        }
        else if (!this->isMinimized())
        {
            Component::Settings::instance()->mainFrameBounds = this->frameGeometry();
            Component::Settings::instance()->mainFrameMaximized = false;
        }
    }
}

void MainFrame::_ensureWithinScreenBounds()
{
    QRect bounds = this->frameGeometry();
    QRect workspaceRect =
        QGuiApplication::primaryScreen()->availableGeometry();
    bounds.setWidth(std::min(bounds.width(), workspaceRect.width()));
    bounds.setHeight(std::min(bounds.height(), workspaceRect.height()));
    bounds.setWidth(std::max(bounds.width(), MinimumSize.width()));
    bounds.setHeight(std::max(bounds.height(), MinimumSize.height()));

    bounds.setX(std::min(bounds.x(), workspaceRect.width() - bounds.width()));
    bounds.setY(std::min(bounds.y(), workspaceRect.height() - bounds.height()));
    bounds.setX(std::max(bounds.x(), workspaceRect.x()));
    bounds.setY(std::max(bounds.y(), workspaceRect.y()));
    _setFrameGeometry(bounds);
}

void MainFrame::_setFrameGeometry(const QRect & bounds)
{
    QRect fg = this->frameGeometry();
    QPoint tl = this->mapToGlobal(QPoint(0, 0));
    this->setGeometry(
        bounds.x() + (tl.x() - fg.x()),
        bounds.y() + (tl.y() - fg.y()),
        bounds.width(),
        bounds.height() - (tl.y() - fg.y()));
}

QWidget * MainFrame::_dialogParent()
{
    return nullptr;
    //  TODO kill off ? return this->isVisible() ? this : nullptr;
}

bool MainFrame::_createWorkspace(
        tt3::ws::WorkspaceAddress workspaceAddress,
        const QString & adminUser,
        const QString & adminLogin,
        const QString & adminPassword
    )
{
    Q_ASSERT(workspaceAddress != nullptr);

    //  If the workspaceAddress refers to the currently
    //  open workspace, the call is an error
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {   //  OOPS!
        tt3::gui::ErrorDialog::show(
            _dialogParent(),
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
            qCritical() << ex;
            tt3::gui::ErrorDialog::show(_dialogParent(), ex);
        }
        //  Replace the "current" workspace
        tt3::gui::theCurrentWorkspace.swap(workspace);
        tt3::ws::Component::Settings::instance()->addRecentWorkspace(workspaceAddress);
        //  TODO kill off _updateMruWorkspaces();
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
                qCritical() << ex;
                tt3::gui::ErrorDialog::show(_dialogParent(), ex);
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
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(_dialogParent(), ex);
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
            qCritical() << ex;
            tt3::gui::ErrorDialog::show(_dialogParent(), ex);
        }
        //  Use the newly open workspace
        tt3::gui::theCurrentWorkspace.swap(workspace);
        tt3::ws::Component::Settings::instance()->addRecentWorkspace(workspaceAddress);
        //  TODO kill off _updateMruWorkspaces();
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
                qCritical() << ex;
                tt3::gui::ErrorDialog::show(_dialogParent(), ex);
                return false;
            }
        }
        //  Done
        refresh();
        return true;
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(_dialogParent(), ex);
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MainFrame));

    //  If the workspaceAddress refers to the currently
    //  open workspace, we can't destroy it
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {
        tt3::gui::ErrorDialog::show(
            _dialogParent(),
            rr.string(
                RID(CannotDestroyWorkspaceError),
                workspaceAddress->displayForm()));
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
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(_dialogParent(), ex);
    }
}

QMenu * MainFrame::_createContextMenu()
{
    QMenu * contextMenu = new QMenu();

    if (_trayIcon == nullptr)
    {   //  Creating context menu for the MainFrame
        contextMenu->addAction(_createActionMinimize());
    }
    else
    {   //  Creating context menu for a system tray icon
        contextMenu->addAction(_createActionRestore());
    }
    contextMenu->addSeparator();

    QMenu * fileMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&File"));
    fileMenu->addAction(_createActionNewWorkspace());
    fileMenu->addAction(_createActionOpenWorkspace());
    fileMenu->addSeparator();
    fileMenu->addAction(_createActionCloseWorkspace());
    fileMenu->addSeparator();
    fileMenu->addAction(_createActionDestroyWorkspace());
    fileMenu->addSeparator();
    fileMenu->addAction(_createActionRestart());

    QMenu * manageMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Manage"));
    manageMenu->setEnabled(tt3::gui::theCurrentWorkspace != nullptr);

    QMenu * toolsMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Tools"));

    QMenu * reportsMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Reports"));
    reportsMenu->setEnabled(tt3::gui::theCurrentWorkspace != nullptr);

    QMenu * optionsMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Options"));
    optionsMenu->addAction(_createActionPreferences());

    QMenu * helpMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Help"));

    contextMenu->addSeparator();
    contextMenu->addAction(_createActionExit());

    return contextMenu;
}

QAction * MainFrame::_createActionMinimize()
{
    QAction * action;
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {   //  Minimize to system tray
        action = new QAction(
            QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeMinimizedSmall.png"),
            TR("&Minimize to system tray"));
    }
    else
    {   //  Minimize to taskbar
        action = new QAction(
            QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeMinimizedSmall.png"),
            TR("&Minimize"));
    }
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionMinimize);
    return action;
}

QAction * MainFrame::_createActionRestore()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeWindowedSmall.png"),
        TR("&Restore"));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionRestore);
    return action;
}

QAction * MainFrame::_createActionPreferences()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/PreferencesSmall.png"),
        TR("&Preferences"));
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionPreferences);
    return action;
}

QAction * MainFrame::_createActionNewWorkspace()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/CreateWorkspaceSmall.png"),
        TR("&New workspace"));
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionNewWorkspace);
    return action;
}

QAction * MainFrame::_createActionOpenWorkspace()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/OpenWorkspaceSmall.png"),
        TR("&Open workspace"));
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionOpenWorkspace);
    return action;
}

QAction * MainFrame::_createActionCloseWorkspace()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/CloseWorkspaceSmall.png"),
        TR("C&lose workspace"));
    action->setEnabled(tt3::gui::theCurrentWorkspace != nullptr);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionCloseWorkspace);
    return action;
}

QAction * MainFrame::_createActionDestroyWorkspace()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/DestroyWorkspaceSmall.png"),
        TR("&Destroy workspace"));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionDestroyWorkspace);
    return action;
}

QAction * MainFrame::_createActionRestart()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/RestartSmall.png"),
        TR("Res&tart TimeTracker3"));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionDestroyWorkspace);
    return action;
}

QAction * MainFrame::_createActionExit()
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/ExitSmall.png"),
        TR("E&xit"));
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionExit);
    return action;
}

//////////
//  Signal handlers
void MainFrame::_trackPositionTimerTimeout()
{
    _loadPosition();
    _trackPosition = true;
}

void MainFrame::_savePositionTimerTimeout()
{
    _savePosition();
}

void MainFrame::_workspaceClosed(tt3::ws::WorkspaceClosedNotification)
{
    refresh();
}

void MainFrame::_currentWorkspaceChanged(tt3::ws::Workspace, tt3::ws::Workspace)
{
    refresh();
}

void MainFrame::_currentCredentialsChanged(tt3::ws::Credentials, tt3::ws::Credentials)
{
    refresh();
}

void MainFrame::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    refresh();
}

void MainFrame::_currentThemeChanged(tt3::gui::ITheme*, tt3::gui::ITheme*)
{
    refresh();
}

void MainFrame::_currentLocaleChanged(QLocale, QLocale)
{
    refresh();
}

void MainFrame::_refreshTimerTimeout()
{
    refresh();
}

void MainFrame::_onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "MainFrame::_onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)";
    if (reason == QSystemTrayIcon::DoubleClick)
    {   //  Restore on double-click
        _onActionRestore();
    }
}

void MainFrame::_onActionMinimize()
{
    qDebug() << "MainFrame::_onActionMinimize()";
    if (this->isVisible() && !this->isMinimized())
    {   //  NOT already minimized
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {   //  Minimize to system tray
            _trayIcon = new QSystemTrayIcon();
            _trayIcon->setIcon(QIcon(":/tt3-skin-slim/Resources/Images/Misc/Tt3Large.png")); // Ensure icon is in Qt resources
            _trayIcon->setToolTip(TR("TimeTracker3"));
            _contextMenu.reset(_createContextMenu());
            _trayIcon->setContextMenu(_contextMenu.get());
            connect(_trayIcon,
                    &QSystemTrayIcon::activated,
                    this,
                    &MainFrame::_onTrayIconActivated);
            _trayIcon->show();
            setVisible(false);
        }
        else
        {   //  Minimize to taskbar
            setWindowState(Qt::WindowMinimized);
        }
    }
}

void MainFrame::_onActionRestore()
{
    delete _trayIcon;   //  "delete nullptr" is safe
    _trayIcon = nullptr;
    setVisible(true);
    setWindowState(Qt::WindowActive);
}

void MainFrame::_onActionNewWorkspace()
{
    tt3::gui::NewWorkspaceDialog dlg(_dialogParent());
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
        _dialogParent(),
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
    //  TODO if would bw nice if the "confirm close"
    //  dialog was displayed BEFORE the "current" workspace
    //  was replaced with nullptr - this will emiminate
    //  the UI flicker.
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
        tt3::gui::ConfirmCloseWorkspaceDialog dlg(_dialogParent(), workspace->address());
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
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(_dialogParent(), ex);
    }
    refresh();
}

void MainFrame::_onActionDestroyWorkspace()
{
    tt3::gui::SelectWorkspaceDialog dlg(
        _dialogParent(),
        tt3::gui::SelectWorkspaceDialog::OptionalControls::None);
    if (dlg.doModal() == tt3::gui::SelectWorkspaceDialog::Result::Ok)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        Q_ASSERT(workspaceAddress != nullptr);
        //  Always confirm!
        tt3::gui::ConfirmDestroyWorkspaceDialog dlg1(this, workspaceAddress);
        if (dlg1.doModal() == tt3::gui::ConfirmDestroyWorkspaceDialog::Result::Yes)
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
    throw tt3::gui::RestartRequest();
}

void MainFrame::_onActionExit()
{   //  Confirm...
    if (tt3::gui::Component::Settings::instance()->confirmExit &&
        !tt3::util::SystemShutdownHandler::isShutdownInProgress())
    {
        tt3::gui::ConfirmExitDialog dlg(_dialogParent());
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

//  End of tt3-skin-slim/MainFrame.cpp
