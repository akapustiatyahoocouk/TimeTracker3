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
    if (event->key() == Qt::Key_M &&
        event->modifiers() == Qt::ControlModifier)
    {
        event->accept();    //  we're handling it!
        _onActionMinimize();
    }
    else if (event->key() == Qt::Key_X &&
             event->modifiers() == Qt::ControlModifier)
    {
        event->accept();    //  we're handling it!
        _onActionExit();
    }
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
    QRect g = this->geometry();
    QRect fg = this->frameGeometry();
    QPoint tl = this->mapToGlobal(QPoint(0, 0));
    this->setGeometry(
        bounds.x() + (tl.x() - fg.x()),
        bounds.y() + (tl.y() - fg.y()),
        bounds.width(),
        bounds.height() - (tl.y() - fg.y()));
}

QMenu * MainFrame::_createContextMenu()
{
    QMenu * contextMenu = new QMenu();

    if (_trayIcon == nullptr)
    {   //  Creating context menu for the MainFrame
        QAction * minimizeAction;
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {   //  Minimize to system tray
            minimizeAction =
                contextMenu->addAction(
                    QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeMinimizedSmall.png"),
                    TR("&Minimize to system tray"));
        }
        else
        {   //  Minimize to taskbar
            minimizeAction =
                contextMenu->addAction(
                    QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeMinimizedSmall.png"),
                    TR("&Minimize"));
        }
        minimizeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
        connect(minimizeAction,
                &QAction::triggered,
                this,
                &MainFrame::_onActionMinimize);
    }
    else
    {   //  Creating context menu for a system tray icon
        QAction * restoreAction =
            contextMenu->addAction(
                QIcon(":/tt3-skin-slim/Resources/Images/Actions/MakeWindowedSmall.png"),
                TR("&Restore"));
        connect(restoreAction,
                &QAction::triggered,
                this,
                &MainFrame::_onActionRestore);
    }
    contextMenu->addSeparator();

    contextMenu->addSeparator();
    QAction * exitAction =
        contextMenu->addAction(
            QIcon(":/tt3-skin-slim/Resources/Images/Actions/ExitSmall.png"),
            TR("E&xit"));
    exitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
    connect(exitAction,
            &QAction::triggered,
            this,
            &MainFrame::_onActionExit);

    return contextMenu;
}

QWidget * MainFrame::_dialogParent()
{
    return nullptr;
    //  TODO kill off ? return this->isVisible() ? this : nullptr;
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

//  End of tt3-skin-slim/MainFrame.cpp
