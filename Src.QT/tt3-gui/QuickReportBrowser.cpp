//
//  tt3-gui/QuickReportBrowser.cpp - tt3::gui::QuickReportBrowser class implementation
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
#include "ui_QuickReportBrowser.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
QuickReportBrowser::QuickReportBrowser(
        QWidget *parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::QuickReportBrowser)
{
    _ui->setupUi(this);

    //  Populate the "quick report" combo box and select
    //  the "current" quick report
    QList<IQuickReport*> quickReportsList = QuickReportManager::allQuickReports().values();
    std::sort(
        quickReportsList.begin(),
        quickReportsList.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    for (IQuickReport * quickReport : quickReportsList)
    {
        _ui->quickReportComboBox->addItem(
            quickReport->smallIcon(),
            quickReport->displayName(),
            QVariant::fromValue(quickReport));
    }

    //  Create dynamic controls
    //  TODO instantiate last used quick report if possible
    _quickReportPanelLayout = new QStackedLayout();
    _quickReportView = DailyWorkQuickReport::instance()->createView(_ui->quickReportPanel);
    _quickReportPanelLayout->addWidget(_quickReportView);
    _ui->quickReportPanel->setLayout(_quickReportPanelLayout);

    //  Refresh & we're done
    _quickReportView->setWorkspace(_workspace);
    _quickReportView->setCredentials(_credentials);
    refresh();
}

QuickReportBrowser::~QuickReportBrowser()
{
    delete _quickReportView;
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace QuickReportBrowser::workspace() const
{
    return _workspace;
}

void QuickReportBrowser::setWorkspace(tt3::ws::Workspace workspace)
{
    _workspace = workspace;
    _quickReportView->setWorkspace(workspace);
}

tt3::ws::Credentials QuickReportBrowser::credentials() const
{
    return _credentials;
}

void QuickReportBrowser::setCredentials(const tt3::ws::Credentials & credentials)
{
    _credentials = credentials;
    _quickReportView->setCredentials(credentials);
}

void QuickReportBrowser::refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        try
        {
            if (_workspace == nullptr || !_credentials.isValid() ||
                !_workspace->isOpen() ||
                !_workspace->canAccess(_credentials)) //  may throw
            {   //  Nothing to show
                _clearAndDisableAllControls();
                return;
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! No point in proceesing.
            qCritical() << ex;
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise browser controls are enabled
        _ui->showLabel->setEnabled(true);
        _ui->quickReportComboBox->setEnabled(true);
        _ui->refreshLabel->setEnabled(true);
        _ui->refreshIntervalComboBox->setEnabled(true);
        _ui->refreshPushButton->setEnabled(true);

        //  The rest is up to the view
        _quickReportView->refresh();
    }
}

void QuickReportBrowser::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  Implementation helpers
IQuickReport * QuickReportBrowser::_selectedQuickReport()
{
    Q_ASSERT(_ui->quickReportComboBox->currentIndex() != -1);
    return _ui->quickReportComboBox->currentData().value<IQuickReport*>();
}

void QuickReportBrowser::_setSelectedQuickReport(IQuickReport * quickReport)
{   //  TODO implement
}

void QuickReportBrowser::_clearAndDisableAllControls()
{
    _ui->showLabel->setEnabled(false);
    _ui->quickReportComboBox->setEnabled(false);
    _ui->refreshLabel->setEnabled(false);
    _ui->refreshIntervalComboBox->setEnabled(false);
    _ui->refreshPushButton->setEnabled(false);
}

//  End of tt3-gui/QuickReportBrowser.cpp
