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
        _ui(new Ui::QuickReportBrowser),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    _applyCurrentLocale();

    //  Create dynamic controls -
    //  Instantiate last used quick report if possible
    Q_ASSERT(_selectedQuickReport() != nullptr);
    _quickReportPanelLayout = new QStackedLayout();
    _quickReportView = _selectedQuickReport()->createView(_ui->quickReportPanel);
    _quickReportPanelLayout->addWidget(_quickReportView);
    _ui->quickReportPanel->setLayout(_quickReportPanelLayout);

    //  Refresh & we're done
    _quickReportView->setWorkspace(_workspace);
    _quickReportView->setCredentials(_credentials);
    refresh();

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &QuickReportBrowser::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &QuickReportBrowser::refreshRequested,
            this,
            &QuickReportBrowser::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start refresing on timer
    connect(
        &_refreshTimer,
        &QTimer::timeout,
        this,
        &QuickReportBrowser::_refreshTimerTimeout);
    if (_selectedRefreshInterval() > 0)
    {
        _refreshTimer.start(_selectedRefreshInterval() * 1000);
    }
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
{
    Q_ASSERT(quickReport != nullptr);

    if (_selectedQuickReport() != quickReport)
    {   //  There IS actually a change
        for (int i = 0; i < _ui->quickReportComboBox->count(); i++)
        {
            if (quickReport == _ui->quickReportComboBox->itemData(i).value<IQuickReport*>())
            {   //  This one!
                _ui->quickReportComboBox->setCurrentIndex(i);
                break;
            }
        }

    }
}

int QuickReportBrowser::_selectedRefreshInterval()
{
    return _ui->refreshIntervalComboBox->currentData().value<int>();
}

void QuickReportBrowser::_setSelectedRefreshInterval(int seconds)
{
    if (_selectedRefreshInterval() != seconds)
    {   //  There IS actually a change
        for (int i = 0; i < _ui->refreshIntervalComboBox->count(); i++)
        {
            if (_ui->refreshIntervalComboBox->itemData(i).value<int>() == seconds)
            {   //  This one!
                _ui->refreshIntervalComboBox->setCurrentIndex(i);
                break;
            }
        }
    }
}

void QuickReportBrowser::_clearAndDisableAllControls()
{
    _ui->showLabel->setEnabled(false);
    _ui->quickReportComboBox->setEnabled(false);
    _ui->refreshLabel->setEnabled(false);
    _ui->refreshIntervalComboBox->setEnabled(false);
    _ui->refreshPushButton->setEnabled(false);
}

void QuickReportBrowser::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(QuickReportBrowser));

    _ui->showLabel->setText(
        rr.string(RID(ShowLabel)));
    _ui->refreshLabel->setText(
        rr.string(RID(RefreshLabel)));
    _ui->refreshPushButton->setText(
        rr.string(RID(RefreshPushButton)));

    //  [re]populate the "quick report" combo box...
    if (auto _ = RefreshGuard(_refreshUnderway))
    {   //  We don't want combo box current item change events handled
        //  while the combo box is initialized
        auto quickReportsList = QuickReportManager::all().values();
        std::sort(
            quickReportsList.begin(),
            quickReportsList.end(),
            [](auto a, auto b)
            {
                return tt3::util::NaturalStringOrder::less(
                    a->displayName(),
                    b->displayName());
            });
        _ui->quickReportComboBox->clear();
        for (IQuickReport * quickReport : std::as_const(quickReportsList))
        {
            _ui->quickReportComboBox->addItem(
                quickReport->smallIcon(),
                quickReport->displayName(),
                QVariant::fromValue(quickReport));
        }
        //  ...and select the "current" quick report
        IQuickReport * quickReport =
            QuickReportManager::find(
                Component::Settings::instance()->quickReport);
        _setSelectedQuickReport(
            (quickReport != nullptr) ?
                quickReport :
                DailyWorkQuickReport::instance());
    }

    //  [Pu]re]populate the "refresh interval" combo box...
    if (auto _ = RefreshGuard(_refreshUnderway))
    {   //  We don't want combo box current item change events handled
        //  while the combo box is initialized
        _ui->refreshIntervalComboBox->clear();
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval1)),
            QVariant::fromValue<int>(1));
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval5)),
            QVariant::fromValue<int>(5));
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval10)),
            QVariant::fromValue<int>(10));
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval15)),
            QVariant::fromValue<int>(15));
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval30)),
            QVariant::fromValue<int>(30));
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval60)),
            QVariant::fromValue<int>(60));
        _ui->refreshIntervalComboBox->addItem(
            rr.string(RID(RefreshInterval0)),
            QVariant::fromValue<int>(0));
        //  ...and select the last used value
        _setSelectedRefreshInterval(
            Component::Settings::instance()->quickReportRefreshInterval);
    }
}

//////////
//  Signal handlers
void QuickReportBrowser::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
}

void QuickReportBrowser::_quickReportComboBoxCurrentIndexhanged(int)
{
    if (!_refreshUnderway)
    {
        Q_ASSERT(_selectedQuickReport() != nullptr);    //  because "daily work" is always present
        Component::Settings::instance()->quickReport = _selectedQuickReport()->mnemonic();
    }
}

void QuickReportBrowser::_refreshIntervalComboBoxCurrentIndexChanged(int)
{
    if (!_refreshUnderway)
    {
        _refreshTimer.stop();
        Component::Settings::instance()->quickReportRefreshInterval = _selectedRefreshInterval();
        if (_selectedRefreshInterval() > 0)
        {
            _refreshTimer.start(_selectedRefreshInterval() * 1000);
        }
    }
}

void QuickReportBrowser::_refreshPushButtonClicked()
{
    refresh();
}

void QuickReportBrowser::_refreshTimerTimeout()
{
    refresh();
}

void QuickReportBrowser::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/QuickReportBrowser.cpp
