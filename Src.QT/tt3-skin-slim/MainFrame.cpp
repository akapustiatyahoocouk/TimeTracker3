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
    _recalculateControlAreas();
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
    _controlAreas.clear();  //  Force recalculate all
    _recalculateControlAreas();
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
        //  Control area click start ?
        for (auto controlArea : _controlAreas)
        {
            if (controlArea->rect.contains(event->pos()))
            {   //  This one!
                controlArea->pressed = true;
                update();
                break;
            }
        }
    }
}

void MainFrame::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        _dragging = false;
        //  Un-press everything
        tt3::ws::Activity activity = nullptr;
        for (auto controlArea : _controlAreas)
        {
            if (controlArea->rect.contains(event->pos()) &&
                controlArea->pressed)
            {
                controlArea->pressed = false;
                //  Did we just un-press an activity ?
                if (auto aca =
                    std::dynamic_pointer_cast<_ActivityAreaImpl>(controlArea))
                {
                    activity = aca->activity;
                }
            }
        }
        update();
        //  Did we do something to the current activity ?
        if (activity != nullptr)
        {
            if (tt3::gui::theCurrentActivity == activity)
            {   //  Yes - we stopped it
                tt3::gui::theCurrentActivity.replaceWith(nullptr);
            }
            else
            {   //  Yes - we started it
                tt3::gui::theCurrentActivity.replaceWith(activity);
            }
            refresh();
        }
        else
        {   //  Just un-pressed the application area
            tt3::gui::theCurrentActivity.replaceWith(nullptr);
            refresh();
        }
    }
}

void MainFrame::mouseMoveEvent(QMouseEvent * event)
{
    if (_dragging)
    {
        int dx = event->pos().x() - _dragAnchor.x();
        int dy = event->pos().y() - _dragAnchor.x();
        if (dx != 0 || dy != 0)
        {
            QRect r = this->frameGeometry();
            this->move(r.x() + dx, r.y() + dy);
            //  Un-press everything
            for (auto controlArea : _controlAreas)
            {
                controlArea->pressed = false;
            }
            update();
        }
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
        { Qt::Key_1, Qt::NoModifier,      &MainFrame::_onActionManageUsers },
        { Qt::Key_2, Qt::NoModifier,      &MainFrame::_onActionManageActivityTypes },
        { Qt::Key_3, Qt::NoModifier,      &MainFrame::_onActionManagePublicActivities },
        { Qt::Key_4, Qt::NoModifier,      &MainFrame::_onActionManagePublicTasks },
        { Qt::Key_5, Qt::NoModifier,      &MainFrame::_onActionManagePrivateActivities },
        { Qt::Key_6, Qt::NoModifier,      &MainFrame::_onActionManagePrivateTasks },
        { Qt::Key_7, Qt::NoModifier,      &MainFrame::_onActionManageProjects },
        { Qt::Key_8, Qt::NoModifier,      &MainFrame::_onActionManageWorkStreams },
        { Qt::Key_9, Qt::NoModifier,      &MainFrame::_onActionManageBeneficiaries },
        { Qt::Key_0, Qt::NoModifier,      &MainFrame::_onActionManageMyDay },
        { Qt::Key_Q, Qt::NoModifier,      &MainFrame::_onActionQuickReports },
        { Qt::Key_L, Qt::ControlModifier, &MainFrame::_onActionLoginAsDifferentUser },
        { Qt::Key_P, Qt::ControlModifier, &MainFrame::_onActionPreferences },
        { Qt::Key_F1,Qt::NoModifier,      &MainFrame::_onActionHelpContent },
        { Qt::Key_F1,Qt::ControlModifier, &MainFrame::_onActionAbout }
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
}

void MainFrame::paintEvent(QPaintEvent * /*event*/)
{
    QPainter p(this);

    for (int i = 0; i < _controlAreas.size(); i++)
    {
        _draw(p, _controlAreas[i]);
    }
}

//////////
//  Operations
void MainFrame::show()
{
    if (!this->isVisible())
    {
        QMainWindow::show();
        _recalculateControlAreas();
        setAlwaysOnTop(Component::Settings::instance()->mainFrameAlwaysOnTop);
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

    //  Control areas
    _recalculateControlAreas();
    //  TODO finish the implementation

    //  Make tray ion refrect main frame's title
    if (_trayIcon != nullptr)
    {
        QString tooltip = title;
        if (gui::theCurrentActivity != nullptr)
        {
            try
            {
                tooltip +=
                    "\n\n" +
                    rr.string(RID(Title.CurrentActivity),
                              gui::theCurrentActivity->displayName(credentials));   //  may throw
                qint64 secs = qMax(0, tt3::gui::theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
                char s[32];
                sprintf(s, " [%d:%02d:%02d]",
                        int(secs / (60 * 60)),
                        int((secs / 60) % 60),
                        int(secs % 60));
                tooltip += s;
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Log & suppress
                qCritical() << ex;
                tooltip += "\n\n" + ex.errorMessage();
            }
        }
        _trayIcon->setToolTip(tooltip);
    }

    //  Done
    update();
}

void MainFrame::setAlwaysOnTop(bool alwaysOnTop)
{
    Qt::WindowFlags oldFlags = windowFlags();
    Qt::WindowFlags newFlags = oldFlags;
    if (alwaysOnTop)
    {
        newFlags |= Qt::WindowStaysOnTopHint;
    }
    else
    {
        newFlags &= ~Qt::WindowStaysOnTopHint;
    }
    if (newFlags != oldFlags)
    {   //  Flags actually change
        if (isVisible())
        {   //  Must hide and re-show for change to take effect
            QMainWindow::hide();
            setWindowFlags(newFlags);
            QMainWindow::show();
        }
        else
        {   //  Not visible - just apply the flags
            setWindowFlags(newFlags);
        }
    }
}

//////////
//  Implementation
void MainFrame::_loadPosition()
{
    _setFrameGeometry(Component::Settings::instance()->mainFrameBounds);
    _ensureWithinScreenBounds();
}

void MainFrame::_savePosition()
{
    if (_trackPosition)
    {
        if (!this->isMinimized() && this->isVisible())
        {
            Component::Settings::instance()->mainFrameBounds = this->frameGeometry();
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
}

void MainFrame::_recalculateControlAreas()
{
    const int _MinControlAreaHeight = 32;

    //  We need to distribute the quick picks list
    QList<tt3::ws::Activity> quickPicks;
    if (gui::theCurrentWorkspace != nullptr)
    {   //  There MAY be a quick picks list for theCurrentCredentials
        try
        {
            auto account = gui::theCurrentWorkspace->login(gui::theCurrentCredentials); //  may throw
            quickPicks = account->quickPicksList(gui::theCurrentCredentials);      //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            quickPicks.clear();
        }
    }
    if (quickPicks.size() + 1 != _controlAreas.size())
    {   //  Must recalculate all!
        _controlAreas.clear();
        QRect r = this->geometry();
        int y = 0, dy = std::max(_MinControlAreaHeight, r.height() / int(quickPicks.size() + 1));
        for (int i = 0; i < quickPicks.size(); i++)
        {
            _controlAreas.append(
                std::make_shared<_ActivityAreaImpl>(
                    QRect(0, y, r.width(), dy),
                    quickPicks[i]));
            y += dy;
        }
        _controlAreas.append(
            std::make_shared<_ApplicationAreaImpl>(
                QRect(0,
                      y,
                      r.width(),
                      std::max(
                        _MinControlAreaHeight,
                        r.bottom() - y - r.y()))));
    }

    //  Make sure each control area represents proper
    //  Activity in a proper state
    for (int i = 0; i < quickPicks.size(); i++)
    {
        auto aa =
            std::dynamic_pointer_cast<_ActivityAreaImpl>(_controlAreas[i]);
        aa->activity = quickPicks[i];
        aa->pressed = (gui::theCurrentActivity == quickPicks[i]);
    }
}

void MainFrame::_draw(QPainter & p, const _ControlArea & controlArea)
{
    QColor tl[5], br[5];

    //  Draw the background
    p.fillRect(controlArea->rect, _labelDecorations.background);
    p.fillRect(controlArea->rect.adjusted(4, 4, -4, -4), controlArea->bias);
    //  Prepare border/fade colors
    tl[0] = controlArea->bias.lighter();
    tl[4] = _labelDecorations.background.lighter();
    tl[2] = tt3::gui::ColorManager::mid(tl[0], tl[4]);
    tl[1] = tt3::gui::ColorManager::mid(tl[0], tl[2]);
    tl[3] = tt3::gui::ColorManager::mid(tl[2], tl[4]);
    br[0] = controlArea->bias.darker();
    br[4] = _labelDecorations.background.darker();
    br[2] = tt3::gui::ColorManager::mid(br[0], br[4]);
    br[1] = tt3::gui::ColorManager::mid(br[0], br[2]);
    br[3] = tt3::gui::ColorManager::mid(br[2], br[4]);
    //  Draw border
    for (int i = 0; i < 5; i++)
    {
        _drawRect3D(
            p,
            controlArea->rect.adjusted(4 - i, 4 - i, i - 4, i - 4),
            controlArea->pressed ? br[i] : tl[i],
            controlArea->pressed ? tl[i] : br[i]);
    }
    //  Draw text
    if (auto aca =
        std::dynamic_pointer_cast<_ActivityAreaImpl>(controlArea))
    {   //  Activity
        try
        {
            p.setFont(
                (tt3::gui::theCurrentActivity == aca->activity) ?
                    _labelDecorations.emphasisFont :
                    _labelDecorations.font);
            p.setPen(_labelDecorations.foreground);
            p.drawText(
                controlArea->rect.adjusted(4, 4, -4, - 4),
                Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap,
                aca->activity->displayName(tt3::gui::theCurrentCredentials));   //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            p.drawText(
                controlArea->rect.adjusted(4, 4, -4, - 4),
                Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap,
                ex.errorMessage());
        }
    }
    else
    {   //  Application
        p.setFont(
            (tt3::gui::theCurrentActivity != nullptr) ?
                _labelDecorations.emphasisFont :
                _labelDecorations.font);
        p.setPen(
            (tt3::gui::theCurrentActivity != nullptr) ?
                _labelDecorations.foreground :
                _labelDecorations.disabledForeground);
        p.drawText(
            controlArea->rect.adjusted(4, 4, -4, - 4),
            Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap,
            TR("Stop current activity"));
    }
}

void MainFrame::_drawRect3D(QPainter & p, const QRect & rc, const QColor & tl, const QColor & br)
{
    p.setPen(tl);
    p.drawLine(rc.x(), rc.y(), rc.right(), rc.y());
    p.drawLine(rc.x(), rc.y(), rc.x(), rc.bottom());
    p.setPen(br);
    p.drawLine(rc.right(), rc.y(), rc.right(), rc.bottom());
    p.drawLine(rc.x(), rc.bottom(), rc.right(), rc.bottom());
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
                _dialogParent(),
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
        tt3::gui::ErrorDialog::show(_dialogParent(), ex);
    }   //  Let Errors and non-tt3 throwns use default behaviour
    refresh();  //  workspace might have changed while report was generated
}

QMenu * MainFrame::_createContextMenu()
{
    QMenu * contextMenu = new QMenu();

    if (_trayIcon == nullptr)
    {   //  Creating context menu for the MainFrame
        contextMenu->addAction(_createActionMinimize(contextMenu));
    }
    else
    {   //  Creating context menu for a system tray icon
        contextMenu->addAction(_createActionRestore(contextMenu));
    }
    contextMenu->addAction(_createActionStopCurrentActivity(contextMenu));
    contextMenu->addSeparator();

    QMenu * fileMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&File"));
    fileMenu->addAction(_createActionNewWorkspace(contextMenu));
    fileMenu->addAction(_createActionOpenWorkspace(contextMenu));
    fileMenu->addSeparator();
    fileMenu->addAction(_createActionCloseWorkspace(contextMenu));
    fileMenu->addSeparator();
    fileMenu->addAction(_createActionDestroyWorkspace(contextMenu));
    fileMenu->addSeparator();
    fileMenu->addAction(_createActionRestart(contextMenu));

    QMenu * manageMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Manage"));
    manageMenu->setEnabled(tt3::gui::theCurrentWorkspace != nullptr);
    manageMenu->addAction(_createActionManageUsers(contextMenu));
    manageMenu->addAction(_createActionManageActivityTypes(contextMenu));
    manageMenu->addAction(_createActionManagePublicActivities(contextMenu));
    manageMenu->addAction(_createActionManagePublicTasks(contextMenu));
    manageMenu->addAction(_createActionManagePrivateActivities(contextMenu));
    manageMenu->addAction(_createActionManagePrivateTasks(contextMenu));
    manageMenu->addAction(_createActionManageProjects(contextMenu));
    manageMenu->addAction(_createActionManageWorkStreams(contextMenu));
    manageMenu->addAction(_createActionManageBeneficiaries(contextMenu));
    manageMenu->addAction(_createActionManageMyDay(contextMenu));

    QMenu * toolsMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Tools"));
    auto tools =
        tt3::util::ToolManager::all().values();
    std::sort(
        tools.begin(),
        tools.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->displayName(), b->displayName());
        });
    for (auto tool : std::as_const(tools))
    {
        toolsMenu->addAction(_createActionInvokeTool(tool, contextMenu));
    }

    QMenu * reportsMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Reports"));
    reportsMenu->setEnabled(tt3::gui::theCurrentWorkspace != nullptr);
    auto reportTypes =
        tt3::report::ReportTypeManager::all().values();
    std::sort(
        reportTypes.begin(),
        reportTypes.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->displayName(), b->displayName());
        });
    for (auto reportType : std::as_const(reportTypes))
    {
        reportsMenu->addAction(_createActionInvokeReport(reportType, contextMenu));
    }
    reportsMenu->addSeparator();
    reportsMenu->addAction(_createActionQuickReports(contextMenu));

    QMenu * optionsMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Options"));
    optionsMenu->addAction(_createActionLoginAsDifferentUser(contextMenu));
    optionsMenu->addSeparator();
    optionsMenu->addAction(_createActionPreferences(contextMenu));

    QMenu * helpMenu =
        contextMenu->addMenu(
            QIcon(":tt3-skin-slim/Resources/Images/Objects/SubmenuSmall.png"),
            TR("&Help"));
    helpMenu->addAction(_createActionHelpContent(contextMenu));
    helpMenu->addAction(_createActionHelpIndex(contextMenu));
    helpMenu->addAction(_createActionHelpSearch(contextMenu));
    helpMenu->addSeparator();
    helpMenu->addAction(_createActionAbout(contextMenu));

    contextMenu->addSeparator();
    contextMenu->addAction(_createActionExit(contextMenu));

    return contextMenu;
}

QAction * MainFrame::_createActionMinimize(QObject * parent)
{
    QAction * action;
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {   //  Minimize to system tray
        action = new QAction(
            QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeMinimizedSmall.png"),
            TR("&Minimize to system tray"),
            parent);
    }
    else
    {   //  Minimize to taskbar
        action = new QAction(
            QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeMinimizedSmall.png"),
            TR("&Minimize"),
            parent);
    }
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionMinimize);
    return action;
}

QAction * MainFrame::_createActionRestore(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeWindowedSmall.png"),
        TR("&Restore"),
        parent);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionRestore);
    return action;
}

QAction * MainFrame::_createActionStopCurrentActivity(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/StopSmall.png"),
        TR("S&top current activity"),
        parent);
    action->setEnabled(gui::theCurrentActivity != nullptr);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionStopCurrentActivity);
    return action;
}

QAction * MainFrame::_createActionNewWorkspace(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/CreateWorkspaceSmall.png"),
        TR("&New workspace"),
        parent);
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionNewWorkspace);
    return action;
}

QAction * MainFrame::_createActionOpenWorkspace(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/OpenWorkspaceSmall.png"),
        TR("&Open workspace"),
        parent);
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionOpenWorkspace);
    return action;
}

QAction * MainFrame::_createActionCloseWorkspace(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/CloseWorkspaceSmall.png"),
        TR("C&lose workspace"),
        parent);
    action->setEnabled(tt3::gui::theCurrentWorkspace != nullptr);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionCloseWorkspace);
    return action;
}

QAction * MainFrame::_createActionDestroyWorkspace(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/DestroyWorkspaceSmall.png"),
        TR("&Destroy workspace"),
        parent);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionDestroyWorkspace);
    return action;
}

QAction * MainFrame::_createActionRestart(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/RestartSmall.png"),
        TR("Res&tart TimeTracker3"),
        parent);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionDestroyWorkspace);
    return action;
}

QAction * MainFrame::_createActionExit(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/ExitSmall.png"),
        TR("E&xit"),
        parent);
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionExit);
    return action;
}

QAction * MainFrame::_createActionManageUsers(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/UserSmall.png"),
        TR("&1 - Users"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_1));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManageUsers);
    return action;
}

QAction * MainFrame::_createActionManageActivityTypes(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/ActivityTypeSmall.png"),
        TR("&2 - Activity types"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_2));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManageActivityTypes);
    return action;
}

QAction * MainFrame::_createActionManagePublicActivities(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/PublicActivitySmall.png"),
        TR("&3 - Public activities"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_3));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManagePublicActivities);
    return action;
}

QAction * MainFrame::_createActionManagePublicTasks(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/PublicTaskSmall.png"),
        TR("&4 - Public tasks"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_4));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManagePublicTasks);
    return action;
}

QAction * MainFrame::_createActionManagePrivateActivities(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/PrivateActivitySmall.png"),
        TR("&5 - Private activities"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_5));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManagePrivateActivities);
    return action;
}

QAction * MainFrame::_createActionManagePrivateTasks(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/PrivateTaskSmall.png"),
        TR("&6 - Private tasks"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_6));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManagePrivateTasks);
    return action;
}

QAction * MainFrame::_createActionManageProjects(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/ProjectSmall.png"),
        TR("&7 - Projects"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_7));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManageProjects);
    return action;
}

QAction * MainFrame::_createActionManageWorkStreams(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/WorkStreamSmall.png"),
        TR("&8 - Work streams"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_8));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManageWorkStreams);
    return action;
}

QAction * MainFrame::_createActionManageBeneficiaries(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Objects/BeneficiarySmall.png"),
        TR("&9 - Beneficiaries"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_9));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManageBeneficiaries);
    return action;
}

QAction * MainFrame::_createActionManageMyDay(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Misc/MyDaySmall.png"),
        TR("&0 - My day"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_0));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionManageMyDay);
    return action;
}

QAction * MainFrame::_createActionQuickReports(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Misc/QuickReportSmall.png"),
        TR("&Quick reports"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_Q));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionQuickReports);
    return action;
}

QAction * MainFrame::_createActionInvokeTool(
        tt3::util::ITool * tool,
        QObject * parent
    )
{
    QAction * action = new QAction(
        tool->smallIcon(),
        tool->displayName(),
        parent);
    action->setEnabled(tool->isEnabled());
    connect(action,
            &QAction::triggered,
            this,
            [=, this]()
            {
                tool->run(_dialogParent());
            });
    return action;
}

QAction * MainFrame::_createActionInvokeReport(
        tt3::report::IReportType * reportType,
        QObject * parent
    )
{
    QAction * action = new QAction(
        reportType->smallIcon(),
        reportType->displayName(),
        parent);
    action->setEnabled(gui::theCurrentWorkspace != nullptr);
    connect(action,
            &QAction::triggered,
            this,
            [=, this]()
            {
                this->_generateReport(reportType);
            });
    return action;
}

QAction * MainFrame::_createActionLoginAsDifferentUser(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/LoginSmall.png"),
        TR("&Login as a different user"),
        parent);
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionLoginAsDifferentUser);
    return action;
}

QAction * MainFrame::_createActionPreferences(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/PreferencesSmall.png"),
        TR("&Preferences"),
        parent);
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionPreferences);
    return action;
}

QAction * MainFrame::_createActionHelpContent(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/HelpContentSmall.png"),
        TR("&Content"),
        parent);
    action->setShortcut(QKeySequence(Qt::Key_F1));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionHelpContent);
    return action;
}

QAction * MainFrame::_createActionHelpIndex(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/HelpIndexSmall.png"),
        TR("&Index"),
        parent);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionHelpIndex);
    return action;
}

QAction * MainFrame::_createActionHelpSearch(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/HelpSearchSmall.png"),
        TR("&Search"),
        parent);
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionHelpSearch);
    return action;
}

QAction * MainFrame::_createActionAbout(QObject * parent)
{
    QAction * action = new QAction(
        QIcon(":/tt3-skin-slim/Resources/Images/Actions/AboutSmall.png"),
        TR("A&bout TimeTracker3"),
        parent);
    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F1));
    connect(action,
            &QAction::triggered,
            this,
            &MainFrame::_onActionAbout);
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
    _labelDecorations = tt3::gui::LabelDecorations();
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
    if (reason == QSystemTrayIcon::DoubleClick)
    {   //  Restore on double-click
        _onActionRestore();
    }
}

void MainFrame::_onActionMinimize()
{
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

void MainFrame::_onActionStopCurrentActivity()
{
    try
    {
        tt3::gui::theCurrentActivity.replaceWith(nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(_dialogParent(), ex);
    }
    refresh();
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

void MainFrame::_onActionManageUsers()
{
    tt3::gui::ManageUsersDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageActivityTypes()
{
    tt3::gui::ManageActivityTypesDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePublicActivities()
{
    tt3::gui::ManagePublicActivitiesDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePublicTasks()
{
    tt3::gui::ManagePublicTasksDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePrivateActivities()
{
    tt3::gui::ManagePrivateActivitiesDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManagePrivateTasks()
{
    tt3::gui::ManagePrivateTasksDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageProjects()
{
    tt3::gui::ManageProjectsDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageWorkStreams()
{
    tt3::gui::ManageWorkStreamsDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageBeneficiaries()
{
    tt3::gui::ManageBeneficiariesDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionManageMyDay()
{
    tt3::gui::ManageMyDayDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionQuickReports()
{
    tt3::gui::QuickReportsDialog dlg(
        _dialogParent(),
        tt3::gui::theCurrentWorkspace,
        tt3::gui::theCurrentCredentials);
    dlg.doModal();
}

void MainFrame::_onActionLoginAsDifferentUser()
{
    tt3::gui::LoginDialog dlg(_dialogParent(), "");
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
                tt3::gui::ConfirmDropWorkspaceDialog dlg1(_dialogParent(), tt3::gui::theCurrentWorkspace->address());
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
            tt3::gui::ErrorDialog::show(_dialogParent(), ex);
        }
        refresh();
    }
}

void MainFrame::_onActionPreferences()
{
    tt3::gui::PreferencesDialog dlg(_dialogParent());
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
    tt3::gui::AboutDialog dlg(_dialogParent());
    dlg.doModal();
}

//  End of tt3-skin-slim/MainFrame.cpp
