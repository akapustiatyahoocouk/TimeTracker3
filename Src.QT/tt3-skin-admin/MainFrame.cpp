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

//#define USE_MODAL_MANAGERS

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
    this->setMinimumSize(MinimumSize);

    _labelDecorations = tt3::gui::LabelDecorations(_ui->currentActivityLabel);
    _applyCurrentLocale();

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

    _manageMyDayTabLayout = new QStackedLayout();
    _myDayManager = new tt3::gui::MyDayManager(_ui->manageMyDayTab);
    _manageMyDayTabLayout->addWidget(_myDayManager);
    _ui->manageMyDayTab->setLayout(_manageMyDayTabLayout);

    _quickReportsTabLayout = new QStackedLayout();
    _quickReportBrowser = new tt3::gui::QuickReportBrowser(_ui->quickReportsTab);
    _quickReportsTabLayout->addWidget(_quickReportBrowser);
    _ui->quickReportsTab->setLayout(_quickReportsTabLayout);

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
    _ui->managersTabWidget->setCurrentIndex(Component::Settings::instance()->mainFrameCurrentTab);
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
    _trackPositionTimer.stop();
    _savePositionTimer.stop();
    _refreshTimer.stop();
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MainFrame));

    tt3::ws::Workspace workspace = tt3::gui::theCurrentWorkspace;
    tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
    //  Frame title
    QString title = rr.string(RID(Title));
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
            title += " " + rr.string(RID(Title.ReadOnlySuffix));
        }
    }
    setWindowTitle(title);

    //  Menu items
    _ui->actionCloseWorkspace->setEnabled(workspace != nullptr);
    _ui->menuView->setEnabled(workspace != nullptr);
    _ui->actionManageUsers->setEnabled(workspace != nullptr);
    _ui->actionManageActivityTypes->setEnabled(workspace != nullptr);
    _ui->actionManagePublicActivities->setEnabled(workspace != nullptr);
    _ui->actionManagePublicTasks->setEnabled(workspace != nullptr);
    _ui->actionManagePrivateActivities->setEnabled(workspace != nullptr);
    _ui->actionManagePrivateTasks->setEnabled(workspace != nullptr);
    _ui->actionManageProjects->setEnabled(workspace != nullptr);
    _ui->actionManageWorkStreams->setEnabled(workspace != nullptr);
    _ui->actionManageBeneficiaries->setEnabled(workspace != nullptr);
    _ui->actionManageMyDay->setEnabled(workspace != nullptr);
    _ui->actionQuickReports->setEnabled(workspace != nullptr);
    _ui->actionRefresh->setEnabled(workspace != nullptr);
    _ui->menuReports->setEnabled(workspace != nullptr);
    _refreshToolsMenuItemAvailability();
    _refreshReportsMenuItemAvailability();

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
    _myDayManager->refresh();
    _quickReportBrowser->refresh();

    _refreshCurrentActivityControls();
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
    QRect g = this->geometry();
    QRect fg = this->frameGeometry();
    QPoint tl = this->mapToGlobal(QPoint(0, 0));
    this->setGeometry(
        bounds.x() + (tl.x() - fg.x()),
        bounds.y() + (tl.y() - fg.y()),
        bounds.width(),
        bounds.height() - (tl.y() - fg.y()));
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
            qCritical() << ex;
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
                qCritical() << ex;
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
        qCritical() << ex;
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
            qCritical() << ex;
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
                qCritical() << ex;
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
        qCritical() << ex;
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MainFrame));

    //  If the workspaceAddress refers to the currently
    //  open workspace, we can't destroy it
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {
        tt3::gui::ErrorDialog::show(
            this,
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

    tt3::ws::WorkspaceAddressesList mru =
        tt3::ws::Component::Settings::instance()->recentWorkspaces;
    for (int i = 0; i < mru.size() && i < 9; i++)
    {
        tt3::ws::WorkspaceAddress a = mru[i];
        QAction * action = menu->addAction(
            a->workspaceType()->smallIcon(),
            "&" + QString(QChar('1' + i)) + " - " + a->displayForm());
        connect(
            action,
            &QAction::triggered,
            this,
            [&,a]()
            {
                if (!_openWorkspace(a, tt3::ws::OpenMode::Default))
                {   //  May want to remove this workspace from the MRU
                    tt3::gui::ConfirmForgetWorkspaceDialog dlg(this, a);
                    if (dlg.doModal() == tt3::gui::ConfirmForgetWorkspaceDialog::Result::Yes)
                    {   //  Do it!
                        tt3::ws::Component::Settings::instance()->removeRecentWorkspace(a);
                        _updateMruWorkspaces();  //  will delete "*this" instance!
                        refresh();
                    }
                }
            });
    }
    _ui->actionRecentWorkspaces->setEnabled(!menu->isEmpty());
}

void MainFrame::_refreshToolsMenu()
{
    _ui->menuTools->clear();    // delete's all QActions
    auto tools = tt3::util::ToolManager::all().values();
    std::sort(
        tools.begin(),
        tools.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->displayName(),
                b->displayName());
        });
    for (auto tool : std::as_const(tools))
    {
        QAction * action = new QAction(
            tool->smallIcon(),
            tool->displayName(),
            _ui->menuTools);
        action->setData(QVariant::fromValue(tool));
        _ui->menuTools->addAction(action);
        connect(action,
                &QAction::triggered,
                this,
                [&]()
                {
                    if (QAction * menuItemAction = qobject_cast<QAction*>( sender()))
                    {
                        auto actionTool = menuItemAction->data().value<tt3::util::ITool*>();
                        try
                        {
                            actionTool->run(this);  //  may throw
                        }
                        catch (const tt3::util::Exception & ex)
                        {
                            qCritical() << ex;
                            tt3::gui::ErrorDialog::show(this, ex);
                        }
                        refresh();
                    }
                });
    }
    _refreshToolsMenuItemAvailability();
}

void MainFrame::_refreshToolsMenuItemAvailability()
{
    bool someToolEnabled = false;
    for (QAction * action : _ui->menuTools->actions())
    {
        auto tool = action->data().value<tt3::util::ITool*>();
        action->setEnabled(tool->isEnabled());
        if (tool->isEnabled())
        {
            someToolEnabled = true;
        }
    }
    _ui->menuTools->setEnabled(someToolEnabled);
}

void MainFrame::_refreshReportsMenu()
{
    //  The last two items in the Reports menu must be
    //  *   A separator, and
    //  *   A "Quick reports" menu item.
    //  All items before that are report items, one per
    //  report type, ant must be destroyed & re-created
    auto actions = _ui->menuReports->actions();
    Q_ASSERT(actions.size() >= 2 &&
             actions[actions.size() - 2]->isSeparator() &&
             !actions[actions.size() - 1]->isSeparator());
    for (int i = 0; i < actions.size() - 2; i++)
    {
        _ui->menuReports->removeAction(actions[i]); //  deletes the action
        delete actions[i];
    }
    //  Need to re-create the Report menu actions
    //  from available report types, sorted by display name
    auto reportTypes = tt3::report::ReportTypeManager::all().values();
    std::sort(
        reportTypes.begin(),
        reportTypes.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(a->displayName(), b->displayName());
        });
    for (int i = 0; i < reportTypes.size(); i++)
    {
        auto action =
            new QAction(
                reportTypes[i]->smallIcon(),
                reportTypes[i]->displayName(),
                _ui->menuReports);
        action->setData(QVariant::fromValue(reportTypes[i]));
        _ui->menuReports->insertAction(
            actions[actions.size() - 2],    //  separator
            action);
        //  Set up report invocation handler
        connect(action,
                &QAction::triggered,
                this,
                [&]()
                {
                    if (QAction * menuItemAction = qobject_cast<QAction*>( sender()))
                    {
                        auto actionReportType = menuItemAction->data().value<tt3::report::IReportType*>();
                        try
                        {
                            _generateReport(actionReportType);  //  may throw
                        }
                        catch (const tt3::util::Exception & ex)
                        {
                            qCritical() << ex;
                            tt3::gui::ErrorDialog::show(this, ex);
                        }
                        refresh();
                    }
                });
    }
    _refreshReportsMenuItemAvailability();
}

void MainFrame::_refreshReportsMenuItemAvailability()
{
    tt3::ws::Workspace workspace = tt3::gui::theCurrentWorkspace;
    tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
    bool canGenerateReports;
    try
    {
        canGenerateReports =
            workspace != nullptr &&
            workspace->grantsAny(   //  may throw
            credentials,
                tt3::ws::Capability::Administrator |
                tt3::ws::Capability::GenerateReports);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log & recover
        qCritical() << ex;
        canGenerateReports = false;
    }
    //  The last two items in the Reports menu must be
    //  *   A separator, and
    //  *   A "Quick reports" menu item.
    //  All items before that are report items, one per
    //  report type, ant must be destroyed & re-created
    auto actions = _ui->menuReports->actions();
    Q_ASSERT(actions.size() >= 2 &&
             actions[actions.size() - 2]->isSeparator() &&
             !actions[actions.size() - 1]->isSeparator());
    for (int i = 0; i < actions.size() - 2; i++)
    {
        actions[i]->setEnabled(canGenerateReports);
    }
}

void MainFrame::_refreshCurrentActivityControls()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MainFrame));

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
            qCritical() << ex;
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

        _ui->currentActivityLabel->setText(
            rr.string(RID(NoCurrentActivityLabel)));
        _ui->currentActivityLabel->setFont(_labelDecorations.font);
        _ui->stopActivityPushButton->setEnabled(false);
    }
    _ui->currentActivityLabel->setPalette(currentActivityLabelPalette);
}

void MainFrame::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MainFrame));

    _ui->menuFile->setTitle(
        rr.string(RID(MenuFile.Title)));
    _ui->menuFile->setTitle(
        rr.string(RID(MenuFile.Title)));
    _ui->menuView->setTitle(
        rr.string(RID(MenuView.Title)));
    _ui->menuTools->setTitle(
        rr.string(RID(MenuTools.Title)));
    _ui->menuReports->setTitle(
        rr.string(RID(MenuReports.Title)));
    _ui->menuOptions->setTitle(
        rr.string(RID(MenuOptions.Title)));
    _ui->menuHelp->setTitle(
        rr.string(RID(MenuHelp.Title)));

    _ui->actionNewWorkspace->setText(
        rr.string(RID(ActionNewWorkspace.Text)));
    _ui->actionNewWorkspace->setToolTip(
        rr.string(RID(ActionNewWorkspace.Tooltip)));
    _ui->actionOpenWorkspace->setText(
        rr.string(RID(ActionOpenWorkspace.Text)));
    _ui->actionOpenWorkspace->setToolTip(
        rr.string(RID(ActionOpenWorkspace.Tooltip)));
    _ui->actionCloseWorkspace->setText(
        rr.string(RID(ActionCloseWorkspace.Text)));
    _ui->actionCloseWorkspace->setToolTip(
        rr.string(RID(ActionCloseWorkspace.Tooltip)));
    _ui->actionDestroyWorkspace->setText(
        rr.string(RID(ActionDestroyWorkspace.Text)));
    _ui->actionDestroyWorkspace->setToolTip(
        rr.string(RID(ActionDestroyWorkspace.Tooltip)));
    _ui->actionRecentWorkspaces->setText(
        rr.string(RID(ActionRecentWorkspaces.Text)));
    _ui->actionRecentWorkspaces->setToolTip(
        rr.string(RID(ActionRecentWorkspaces.Tooltip)));
    _ui->actionRestart->setText(
        rr.string(RID(ActionRestart.Text)));
    _ui->actionRestart->setToolTip(
        rr.string(RID(ActionRestart.Tooltip)));
    _ui->actionExit->setText(
        rr.string(RID(ActionExit.Text)));
    _ui->actionExit->setToolTip(
        rr.string(RID(ActionExit.Tooltip)));

    _ui->actionManageUsers->setText(
        rr.string(RID(ActionManageUsers.Text)));
    _ui->actionManageUsers->setToolTip(
        rr.string(RID(ActionManageUsers.Tooltip)));
    _ui->actionManageActivityTypes->setText(
        rr.string(RID(ActionManageActivityTypes.Text)));
    _ui->actionManageActivityTypes->setToolTip(
        rr.string(RID(ActionManageActivityTypes.Tooltip)));
    _ui->actionManagePublicActivities->setText(
        rr.string(RID(ActionManagePublicActivities.Text)));
    _ui->actionManagePublicActivities->setToolTip(
        rr.string(RID(ActionManagePublicActivities.Tooltip)));
    _ui->actionManagePublicTasks->setText(
        rr.string(RID(ActionManagePublicTasks.Text)));
    _ui->actionManagePublicTasks->setToolTip(
        rr.string(RID(ActionManagePublicTasks.Tooltip)));
    _ui->actionManagePrivateActivities->setText(
        rr.string(RID(ActionManagePrivateActivities.Text)));
    _ui->actionManagePrivateActivities->setToolTip(
        rr.string(RID(ActionManagePrivateActivities.Tooltip)));
    _ui->actionManagePrivateTasks->setText(
        rr.string(RID(ActionManagePrivateTasks.Text)));
    _ui->actionManagePrivateTasks->setToolTip(
        rr.string(RID(ActionManagePrivateTasks.Tooltip)));
    _ui->actionManageProjects->setText(
        rr.string(RID(ActionManageProjects.Text)));
    _ui->actionManageProjects->setToolTip(
        rr.string(RID(ActionManageProjects.Tooltip)));
    _ui->actionManageWorkStreams->setText(
        rr.string(RID(ActionManageWorkStreams.Text)));
    _ui->actionManageWorkStreams->setToolTip(
        rr.string(RID(ActionManageWorkStreams.Tooltip)));
    _ui->actionManageBeneficiaries->setText(
        rr.string(RID(ActionManageBeneficiaries.Text)));
    _ui->actionManageBeneficiaries->setToolTip(
        rr.string(RID(ActionManageBeneficiaries.Tooltip)));
    _ui->actionManageMyDay->setText(
        rr.string(RID(ActionManageMyDay.Text)));
    _ui->actionManageMyDay->setToolTip(
        rr.string(RID(ActionManageMyDay.Tooltip)));
    _ui->actionRefresh->setText(
        rr.string(RID(ActionRefresh.Text)));
    _ui->actionRefresh->setToolTip(
        rr.string(RID(ActionRefresh.Tooltip)));

    _ui->actionQuickReports->setText(
        rr.string(RID(ActionQuickReports.Text)));
    _ui->actionQuickReports->setToolTip(
        rr.string(RID(ActionQuickReports.Tooltip)));

    _ui->actionLoginAsDifferentUser->setText(
        rr.string(RID(ActionLoginAsDifferentUser.Text)));
    _ui->actionLoginAsDifferentUser->setToolTip(
        rr.string(RID(ActionLoginAsDifferentUser.Tooltip)));
    _ui->actionPreferences->setText(
        rr.string(RID(ActionPreferences.Text)));
    _ui->actionPreferences->setToolTip(
        rr.string(RID(ActionPreferences.Tooltip)));

    _ui->actionHelpContent->setText(
        rr.string(RID(ActionHelpContent.Text)));
    _ui->actionHelpContent->setToolTip(
        rr.string(RID(ActionHelpContent.Tooltip)));
    _ui->actionHelpIndex->setText(
        rr.string(RID(ActionHelpIndex.Text)));
    _ui->actionHelpIndex->setToolTip(
        rr.string(RID(ActionHelpIndex.Tooltip)));
    _ui->actionHelpSearch->setText(
        rr.string(RID(ActionHelpSearch.Text)));
    _ui->actionHelpSearch->setToolTip(
        rr.string(RID(ActionHelpSearch.Tooltip)));
    _ui->actionAbout->setText(
        rr.string(RID(ActionAbout.Text)));
    _ui->actionAbout->setToolTip(
        rr.string(RID(ActionAbout.Tooltip)));

    _ui->managersTabWidget->setTabText(
        0, rr.string(RID(ManageUsersTab.Text)));
    _ui->managersTabWidget->setTabText(
        1, rr.string(RID(ManageActivityTypesTab.Text)));
    _ui->managersTabWidget->setTabText(
        2, rr.string(RID(ManagePublicActivitiesTab.Text)));
    _ui->managersTabWidget->setTabText(
        3, rr.string(RID(ManagePublicTasksTab.Text)));
    _ui->managersTabWidget->setTabText(
        4, rr.string(RID(ManagePrivateActivitiesTab.Text)));
    _ui->managersTabWidget->setTabText(
        5, rr.string(RID(ManagePrivateTasksTab.Text)));
    _ui->managersTabWidget->setTabText(
        6, rr.string(RID(ManageProjectsTab.Text)));
    _ui->managersTabWidget->setTabText(
        7, rr.string(RID(ManageWorkStreamsTab.Text)));
    _ui->managersTabWidget->setTabText(
        8, rr.string(RID(ManageBeneficiariesTab.Text)));
    _ui->managersTabWidget->setTabText(
        9, rr.string(RID(ManageMyDayTab.Text)));
    _ui->managersTabWidget->setTabText(
        10, rr.string(RID(QuickReportsTab.Text)));

    _ui->stopActivityPushButton->setText(
        rr.string(RID(StopActivityPushButton)));

    _refreshToolsMenu();
    _refreshReportsMenu();
}

void MainFrame::_generateReport(
        tt3::report::IReportType * reportType
    )
{
    tt3::ws::Workspace workspace = tt3::gui::theCurrentWorkspace;
    tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
    try
    {   //  We need outer handlers for report credentials manipulation...
        tt3::ws::ReportCredentials reportCredentials =
            workspace->beginReport( //  may throw
                credentials,
                workspace->objectCount(credentials) * 85 + //  1,000,000 objects -> 1 day lease...
                60 * 60 * 1000);    //  ...+ 1 hour
        try
        {   //  ...and inner handler for report job
            tt3::report::CreateReportDialog dlg(
                this,
                workspace,
                reportCredentials,
                reportType);
            //  At the moment we don't care about doModal() results
            dlg.doModal();
            workspace->releaseCredentials(reportCredentials);   //  may throw
        }
        catch (...)
        {   //  OOPS! Cleanup & re-throw
            workspace->releaseCredentials(reportCredentials);   //  may throw
            throw;
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Report & we're done
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
    }   //  Let Errors and non-tt3 throwns use default behaviour
    refresh();  //  workspace might have changed while report was generated
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
        qCritical() << ex;
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
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManageUsersDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(0);
#endif
}

void MainFrame::_onActionManageActivityTypes()
{
#ifdef USE_MODAL_MANAGERS
        tt3::gui::ManageActivityTypesDialog dlg(
            this,
            tt3::gui::theCurrentWorkspace,
            tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(1);
#endif
}

void MainFrame::_onActionManagePublicActivities()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManagePublicActivitiesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(2);
#endif
}

void MainFrame::_onActionManagePublicTasks()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManagePublicTasksDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(3);
#endif
}

void MainFrame::_onActionManagePrivateActivities()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManagePrivateActivitiesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(4);
#endif
}

void MainFrame::_onActionManagePrivateTasks()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManagePrivateTasksDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(5);
#endif
}

void MainFrame::_onActionManageProjects()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManageProjectsDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(6);
#endif
}

void MainFrame::_onActionManageWorkStreams()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManageWorkStreamsDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(7);
#endif
}

void MainFrame::_onActionManageBeneficiaries()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManageBeneficiariesDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(8);
#endif
}

void MainFrame::_onActionManageMyDay()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::ManageMyDayDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(9);
#endif
}

void MainFrame::_onActionQuickReports()
{
#ifdef USE_MODAL_MANAGERS
    tt3::gui::QuickReportsDialog dlg(
        this,
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
#else
    _ui->managersTabWidget->setCurrentIndex(10);
#endif
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
            qCritical() << ex;
            tt3::gui::ErrorDialog::show(this, ex);
        }
    }
    refresh();
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
            qCritical() << ex;
            tt3::gui::ErrorDialog::show(this, ex);
        }
        refresh();
    }
}

void MainFrame::_onActionPreferences()
{
    tt3::gui::PreferencesDialog dlg(this);
    dlg.doModal();
}

void MainFrame::_onActionHelpContent()
{
    tt3::gui::HelpClient::showContents();
}

void MainFrame::_onActionHelpIndex()
{
    tt3::gui::HelpClient::showIndex();
}

void MainFrame::_onActionHelpSearch()
{
    tt3::gui::HelpClient::showSearch();
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
        qCritical() << ex;
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
    _myDayManager->setWorkspace(tt3::gui::theCurrentWorkspace);
    _quickReportBrowser->setWorkspace(tt3::gui::theCurrentWorkspace);
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
    _myDayManager->setCredentials(tt3::gui::theCurrentCredentials);
    _quickReportBrowser->setCredentials(tt3::gui::theCurrentCredentials);
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

void MainFrame::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
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
