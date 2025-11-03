//
//  tt3-gui/FullScreenReminderWindow.cpp - tt3::gui::FullScreenReminderWindow class implementation
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
#include "tt3-gui/API.hpp"
#include "ui_FullScreenReminderWindow.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentActivity theCurrentActivity;
}

QFont doubleFontSize(QFont font)
{
    QFont result = font;
    result.setPointSize(font.pointSize() * 2);
    return result;
}

//////////
//  Construction/destruction
FullScreenReminderWindow::FullScreenReminderWindow(
    ) : QDialog(nullptr),
        _ui(new Ui::FullScreenReminderWindow),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    _labelDecorations = LabelDecorations(_ui->currentActivityLabel);
    _pushButtonDecorations = PushButtonDecorations(_ui->stopPushButton);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &FullScreenReminderWindow::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Prepare to refresh on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &FullScreenReminderWindow::_refreshTimerTimeout);
}

FullScreenReminderWindow::~FullScreenReminderWindow()
{
    delete _ui;
}

//////////
//  QWidget
void FullScreenReminderWindow::showEvent(QShowEvent * event)
{
    QDialog::showEvent(event);
    _refreshTimer.start(1000);
}

void FullScreenReminderWindow::hideEvent(QHideEvent * event)
{
    QDialog::hideEvent(event);
    _refreshTimer.stop();
}

//////////
//  Operations
void FullScreenReminderWindow::refresh()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(FullScreenReminderWindow));
    _ui->stopPushButton->setText(
        rr.string(RID(StopPushButton)));

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, _labelDecorations.liveStatusBackground);
    palette.setColor(QPalette::WindowText, _labelDecorations.foreground);
    if (theCurrentActivity != nullptr)
    {
        qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
        char s[32];
        sprintf(s, " [%d:%02d:%02d]",
                int(secs / (60 * 60)),
                int((secs / 60) % 60),
                int(secs % 60));
        try
        {
            _ui->currentActivityLabel->setText(
                theCurrentActivity->displayName(theCurrentCredentials));
            _ui->durationLabel->setText(s);
            _ui->stopPushButton->setEnabled(true);

            _ui->currentActivityLabel->setFont(
                doubleFontSize(_labelDecorations.emphasisFont));
            _ui->durationLabel->setFont(
                doubleFontSize(_labelDecorations.emphasisFont));
            _pushButtonDecorations.applyTo(
                _ui->stopPushButton,
                PushButtonDecorations::ButtonRole::LiveStatusButton);
        }
        catch (const tt3::util::Exception & ex)
        {
            _ui->currentActivityLabel->setText(ex.errorMessage());
            _ui->durationLabel->setText("");
            _ui->stopPushButton->setEnabled(false);

            palette.setColor(QPalette::Window, _labelDecorations.errorBackground);
            palette.setColor(QPalette::WindowText, _labelDecorations.errorForeground);

            _ui->currentActivityLabel->setFont(
                doubleFontSize(_labelDecorations.font));
            _ui->durationLabel->setFont(
                doubleFontSize(_labelDecorations.font));
            _pushButtonDecorations.applyTo(
                _ui->stopPushButton,
                PushButtonDecorations::ButtonRole::ErrorButton);
        }
    }
    else
    {
        _ui->currentActivityLabel->setText("");
        _ui->durationLabel->setText("");
        _ui->stopPushButton->setEnabled(false);

        _ui->currentActivityLabel->setFont(
            doubleFontSize(_labelDecorations.font));
        _ui->durationLabel->setFont(
            doubleFontSize(_labelDecorations.font));
        _pushButtonDecorations.applyTo(
            _ui->stopPushButton,
            PushButtonDecorations::ButtonRole::DisabledButton);
    }
    this->setPalette(palette);
}

//////////
//  Signal handlers
void FullScreenReminderWindow::_currentThemeChanged(ITheme *, ITheme *)
{
    _labelDecorations = LabelDecorations(_ui->currentActivityLabel);
    _pushButtonDecorations = PushButtonDecorations(_ui->stopPushButton);
    refresh();
}

void FullScreenReminderWindow::_stopPushButtonClicked()
{
    theCurrentActivity.replaceWith(nullptr);
}

void FullScreenReminderWindow::_refreshTimerTimeout()
{
    refresh();
}

void FullScreenReminderWindow::accept()
{   //  Don't close on Enter
}

void FullScreenReminderWindow::reject()
{   //  Don't close on Esc or Alt+F4
}

//  End of tt3-gui/FullScreenReminderWindow.cpp
