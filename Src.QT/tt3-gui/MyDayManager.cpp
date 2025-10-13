//
//  tt3-gui/MyDayManager.cpp - tt3::gui::MyDayManager class implementation
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
#include "ui_MyDayManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
    extern CurrentActivity theCurrentActivity;
}

//////////
//  Construction/destruction
MyDayManager::MyDayManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  View model
        _myDayModel(new _MyDayModelImpl()),
        //  Controls
        _ui(new Ui::MyDayManager),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    _pushButtonDecorations = PushButtonDecorations(_ui->quickPicksPushButton);

    //  Populate the "quick pick" buttons area
    _recreateDynamicControls();

    //  Populate the "filter" combo box
    _ui->filterComboBox->addItem("today", QVariant::fromValue(1));
    _ui->filterComboBox->addItem("last 2 days", QVariant::fromValue(2));
    _ui->filterComboBox->addItem("last 3 days", QVariant::fromValue(3));
    _ui->filterComboBox->addItem("last 7 days", QVariant::fromValue(7));
    _ui->filterComboBox->addItem("last 30 days", QVariant::fromValue(30));

    //  Populate the ",my day" view area
    _myDayModel = _createMyDayModel();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &MyDayManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &MyDayManager::refreshRequested,
            this,
            &MyDayManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Current activity change means, at least, a refresh
    connect(&theCurrentActivity,
            &CurrentActivity::changed,
            this,
            &MyDayManager::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();

    //  Must listen to refresh timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &MyDayManager::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

MyDayManager::~MyDayManager()
{
    _refreshTimer.stop();
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace MyDayManager::workspace() const
{
    return _workspace;
}

void MyDayManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        _recreateDynamicControls();
        _myDayModel = _createMyDayModel();
        requestRefresh();
    }
}

tt3::ws::Credentials MyDayManager::credentials() const
{
    return _credentials;
}

void MyDayManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        _recreateDynamicControls();
        _myDayModel = _createMyDayModel();
        requestRefresh();
    }
}

void MyDayManager::refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    RefreshGuard refreshGuard(refreshUnderway);
    if (refreshGuard)   //  Don't recurse!
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
            qCritical() << ex.errorMessage();
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise some controls are always enabled...
        _ui->quickPicksPushButton->setEnabled(
            !_workspace->isReadOnly());
        _ui->filterLabel->setEnabled(true);
        _ui->filterComboBox->setEnabled(true);
        try
        {
            _ui->logEventPushButton->setEnabled(
                !_workspace->isReadOnly() &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                    tt3::ws::Capability::LogEvents));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->logEventPushButton->setEnabled(false);
        }

        _refreslLogList();

        //  TODO finish the implementation

        //  TODO adjust for RO

        //  Adjust quick pick button states & appearance
        for (int i = 0; i < _quickPicksList.size(); i++)
        {
            try
            {
                if (theCurrentActivity == _quickPicksList[i])
                {   //  Must adjust the button's text & UI style
                    qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
                    char s[32];
                    sprintf(s, " [%d:%02d:%02d]",
                            int(secs / (60 * 60)),
                            int((secs / 60) % 60),
                            int(secs % 60));
                    _quickPicksButtons[i]->setText(
                        _quickPicksList[i]->displayName(_credentials) + s); //  may throw
                    bool canStop =
                        !_workspace->isReadOnly() &&
                        _quickPicksList[i]->canStop(_credentials); //  may throw
                    _pushButtonDecorations.applyTo(
                        _quickPicksButtons[i],
                        canStop ?
                            PushButtonDecorations::ButtonRole::LiveStatusButton :
                            PushButtonDecorations::ButtonRole::DisabledButton);
                    _quickPicksButtons[i]->setEnabled(canStop);
                }
                else
                {   //  Must adjust the button's text & UI style
                    _quickPicksButtons[i]->setText(
                        _quickPicksList[i]->displayName(_credentials)); //  may throw
                    bool canStart =
                        !_workspace->isReadOnly() &&
                        _quickPicksList[i]->canStart(_credentials); //  may throw
                    _pushButtonDecorations.applyTo(
                        _quickPicksButtons[i],
                        canStart ?
                            PushButtonDecorations::ButtonRole::NormalButton :
                            PushButtonDecorations::ButtonRole::DisabledButton);
                    _quickPicksButtons[i]->setEnabled(canStart);
                }
            }
            catch (const tt3::util::Exception & ex)
            {
                _quickPicksButtons[i]->setText(ex.errorMessage());
                _pushButtonDecorations.applyTo(
                    _quickPicksButtons[i],
                    PushButtonDecorations::ButtonRole::ErrorButton);
                _quickPicksButtons[i]->setEnabled(false);
            }
        }
    }
}

void MyDayManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
MyDayManager::_MyDayModel MyDayManager::_createMyDayModel()
{
    _MyDayModel myDayModel { new _MyDayModelImpl() };
    if (_workspace != nullptr)
    {
        try
        {
            tt3::ws::Account account = _workspace->login(_credentials); //  may throw
            for (tt3::ws::Work work : account->works(_credentials)) //  may throw
            {
                try
                {
                    myDayModel->itemModels.append(_createWorkModel(work));  //  may throw
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log, but ignore
                    qCritical() << ex.errorMessage();
                }
            }
            //  If there is a "current" activity add its item
            if (theCurrentActivity != nullptr)
            {
                myDayModel->itemModels.append(
                    _CurrentActivityModel(
                        new _CurrentActivityModelImpl(
                            theCurrentActivity,
                            theCurrentActivity.lastChangedAt().toLocalTime(),
                            theCurrentActivity->displayName(_credentials),  //  may throw
                            theCurrentActivity->type()->smallIcon())));     //  may throw
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex.errorMessage();
            myDayModel->itemModels.clear();
            //  TODO add a single "error" _ItemModel
        }
    }
    _breakLongWorks(myDayModel);
    _addDateIndicators(myDayModel);
    _sortChronologically(myDayModel);
    qDebug() << "==========";
    for (auto itemModel : myDayModel->itemModels)
    {
        qDebug() << itemModel->toString();
    }
    return myDayModel;
}

MyDayManager::_WorkModel MyDayManager::_createWorkModel(tt3::ws::Work work)
{
    _WorkModel workModel
        { new _WorkModelImpl(
            work,
            work->startedAt(_credentials).toLocalTime(),            //  may throw
            work->finishedAt(_credentials).toLocalTime(),           //  may throw
            work->activity(_credentials)->displayName(_credentials),//  may throw
            work->activity(_credentials)->type()->smallIcon()) };   //  may throw
    return workModel;
}

void MyDayManager::_breakLongWorks(_MyDayModel myDayModel)
{
    for (int i = 0; i < myDayModel->itemModels.size(); i++)
    {
        if (_WorkModel workModel = std::dynamic_pointer_cast<_WorkModelImpl>(myDayModel->itemModels[i]))
        {
            QDate startedOn = workModel->startedAt().date();
            QDate finishedOn = workModel->finishedAt().date();
            if (startedOn < finishedOn)
            {   //  Limit to "startedOn" date, splitting the rest of the
                //  work into the next date
                QDateTime headStartedAt = workModel->startedAt();
                QDateTime headFinishedAt(startedOn, QTime(23, 59, 59, 999));
                _WorkModel head
                    { new _WorkModelImpl(
                        workModel->work(),
                        headStartedAt,
                        headFinishedAt,
                        workModel->displayName(),
                        workModel->icon()) };
                QDateTime tailStartedAt(startedOn.addDays(1), QTime(0, 0));
                QDateTime tailFinishedAt = workModel->finishedAt();
                _WorkModel tail
                    { new _WorkModelImpl(
                        workModel->work(),
                        tailStartedAt,
                        tailFinishedAt,
                        workModel->displayName(),
                        workModel->icon())};
                myDayModel->itemModels[i] = head;
                myDayModel->itemModels.insert(i + 1, tail);
            }
        }
    }
}

void MyDayManager::_addDateIndicators(_MyDayModel myDayModel)
{
    if (myDayModel->itemModels.isEmpty())
    {   //  Nothing to do
        return;
    }
    QDate today = QDateTime::currentDateTime().date();
    QDate minDate = myDayModel->itemModels[0]->startedAt().date();
    QDate maxDate = myDayModel->itemModels[0]->finishedAt().date();
    QSet<QDate> datesWithWorks;
    for (int i = 0; i < myDayModel->itemModels.size(); i++)
    {
        minDate = qMin(minDate, myDayModel->itemModels[i]->startedAt().date());
        maxDate = qMax(maxDate, myDayModel->itemModels[i]->finishedAt().date());
        datesWithWorks.insert(minDate);
        datesWithWorks.insert(maxDate);
    }
    if (minDate == maxDate)
    {   //  We need at most one date indicator...
        if (minDate == today)
        {   //  ...which is optional
            return;
        }
        myDayModel->itemModels.append(
            _DateModel(new _DateModelImpl(minDate)));
        return;
    }
    Q_ASSERT(minDate < maxDate);
    for (QDate d = minDate; d <= maxDate; d = d.addDays(1))
    {
        if (datesWithWorks.contains(d))
        {
            myDayModel->itemModels.append(
                _DateModel(new _DateModelImpl(d)));
        }
    }
}

void MyDayManager::_sortChronologically(_MyDayModel myDayModel)
{
    std::sort(
        myDayModel->itemModels.begin(),
        myDayModel->itemModels.end(),
        [](auto a, auto b)
        {   //  We need REVERSE sorting!
            return a->startedAt() > b->startedAt();
        });
}

//////////
//  Implementation helpers
void MyDayManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &MyDayManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &MyDayManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &MyDayManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &MyDayManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void MyDayManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &MyDayManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &MyDayManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &MyDayManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &MyDayManager::_objectModified);
    }
}

void MyDayManager::_clearAndDisableAllControls()
{
    _ui->quickPicksPushButton->setEnabled(false);
    _ui->filterLabel->setEnabled(false);
    _ui->filterComboBox->setEnabled(false);
    _ui->logEventPushButton->setEnabled(false);
    _myDayModel->clear();
    //  TODO finish the implementation
}

void MyDayManager::_recreateQuickPickButtons()
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    for (int i = 0; i < _quickPicksButtons.size(); i++)
    {
        delete _quickPicksButtons[i];
    }
    _quickPicksButtons.clear();
    //  No quick picks buttons is a special case
    if (_quickPicksList.isEmpty())
    {
        _ui->noQuickPicksLabel->setVisible(true);
        return;
    }
    //  Create buttons
    for (int i = 0; i < _quickPicksList.size(); i++)
    {
        QPushButton * button;
        try
        {
            button = new QPushButton(
                _quickPicksList[i]->type()->smallIcon(),
                _quickPicksList[i]->displayName(_credentials),   //  may throw
                this);
        }
        catch (const tt3::util::Exception & ex)
        {
            button = new QPushButton(
                errorIcon,
                ex.errorMessage(),
                this);
        }
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _quickPicksButtons.append(button);
        _ui->quickPicksPanelLayout->addWidget(button);
        connect(button,
                &QPushButton::clicked,
                this,
                &MyDayManager::_quickPickPushButtonClicked);
    }
    _ui->noQuickPicksLabel->setVisible(false);
}

void MyDayManager::_recreateDynamicControls()
{
    if (_workspace != nullptr)
    {
        try
        {
            _quickPicksList = _workspace->login(_credentials)->quickPicksList(_credentials);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS!. Log, but suppress
            qCritical() << ex.errorMessage();
            _quickPicksList.clear();
        }
    }
    else
    {
        _quickPicksList.clear();
    }
    _recreateQuickPickButtons();
}

void MyDayManager::_refreslLogList()
{
    while (_ui->logListWidget->count() < _myDayModel->itemModels.size())
    {   //  Too few items in the log list
        _ui->logListWidget->addItem(new QListWidgetItem());
    }
    while (_ui->logListWidget->count() > _myDayModel->itemModels.size())
    {   //  Too many items in the log list
        delete _ui->logListWidget->takeItem(_ui->logListWidget->count() - 1);
    }
    for (int i = 0; i < _myDayModel->itemModels.size(); i++)
    {
        _ui->logListWidget->item(i)->setText(_myDayModel->itemModels[i]->toString());
        _ui->logListWidget->item(i)->setIcon(_myDayModel->itemModels[i]->icon());
    }
}

//////////
//  Signal handlers
void MyDayManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _pushButtonDecorations = PushButtonDecorations(_ui->quickPicksPushButton);
    requestRefresh();
}

void MyDayManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    _recreateDynamicControls();
    _myDayModel = _createMyDayModel();
    requestRefresh();
}

void MyDayManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    _recreateDynamicControls();
    _myDayModel = _createMyDayModel();
    requestRefresh();
}

void MyDayManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    _recreateDynamicControls();
    _myDayModel = _createMyDayModel();
    requestRefresh();
}

void MyDayManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    _recreateDynamicControls();
    _myDayModel = _createMyDayModel();
    requestRefresh();
}

void MyDayManager::_refreshRequested()
{
    refresh();
}

void MyDayManager::_quickPicksPushButtonClicked()
{
    try
    {
        tt3::ws::Account account = _workspace->login(_credentials);
        ManageQuickPicksListDialog dlg(this, account, _credentials);
        if (dlg.doModal() == ManageQuickPicksListDialog::Result::Ok)
        {   //  Refresh quick picks buttons
            account->setQuickPicksList(_credentials, dlg.quickPicksList());
            _recreateDynamicControls();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void MyDayManager::_logEventPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void MyDayManager::_quickPickPushButtonClicked()
{
    QPushButton * senderButton =
        dynamic_cast<QPushButton*>(sender());
    for (int i = 0; i < _quickPicksButtons.size(); i++)
    {
        if (_quickPicksButtons[i] == senderButton)
        {
            theCurrentActivity.replaceWith(_quickPicksList[i]);
            requestRefresh();
            break;
        }
    }
}

void MyDayManager::_refreshTimerTimeout()
{
    if (theCurrentActivity != nullptr)
    {
        requestRefresh();
    }
}

void MyDayManager::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    _myDayModel = _createMyDayModel();
    requestRefresh();
}

//  End of tt3-gui/MyDayManager.cpp
