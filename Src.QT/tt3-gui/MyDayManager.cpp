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
    _listWidgetDecorations = ListWidgetDecorations(_ui->logListWidget);
    _applyCurrentLocale();
    _setLogDepth(Component::Settings::instance()->myDayLogDepth);

    //  Populate the "quick pick" buttons area
    _recreateDynamicControls();

    //  Populate the "my day" view area
    _myDayModel = _createMyDayModel();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &MyDayManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &MyDayManager::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  View options changes should cause a refresh
    connect(&Component::Settings::instance()->myDayLogDepth,
            &tt3::util::AbstractSetting::valueChanged,
            this,
            &MyDayManager::_viewOptionSettingValueChanged,
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
    
    //  Done
    _constructed = true;
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MyDayManager));

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

        //  Otherwise some controls are always enabled...
        _ui->quickPicksPushButton->setEnabled(
            !_workspace->isReadOnly());
        _ui->filterLabel->setEnabled(true);
        _ui->filterComboBox->setEnabled(true);
        _ui->logListWidget->setEnabled(true);
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
            qCritical() << ex;
            _ui->logEventPushButton->setEnabled(false);
        }

        _refreslLogList();

        //  Adjust quick pick button states & appearance
        for (int i = 0; i < _quickPicksList.size(); i++)
        {
            try
            {
                QString suffix;
                if (tt3::ws::Task task =
                    std::dynamic_pointer_cast<tt3::ws::TaskImpl>(_quickPicksList[i]))
                {
                    if (task->completed(_credentials))
                    {
                        suffix = "\n" + rr.string(RID(TaskCompletedSuffix));
                    }
                }
                if (theCurrentActivity == _quickPicksList[i])
                {   //  Must adjust the button's text & UI style
                    qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
                    char s[32];
                    sprintf(s, " [%d:%02d:%02d]",
                            int(secs / (60 * 60)),
                            int((secs / 60) % 60),
                            int(secs % 60));
                    _quickPicksButtons[i]->setText(
                        _quickPicksList[i]->displayName(_credentials) + s + suffix); //  may throw
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
                        _quickPicksList[i]->displayName(_credentials) + suffix); //  may throw
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
                qCritical() << ex;
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
    _MyDayModel myDayModel = std::make_shared<_MyDayModelImpl>();
    if (_workspace != nullptr)
    {
        QDate localToday = QDateTime::currentDateTime().date();
        QDateTime localFrom(localToday.addDays(-_logDepth() + 1), QTime(0, 0));
        QDateTime localTo(localToday, QTime(23, 59, 59, 999));
        QDateTime from = localFrom.toUTC(), to = localTo.toUTC();
        try
        {
            tt3::ws::Account account = _workspace->login(_credentials); //  may throw
            for (const auto & work : account->works(_credentials, from, to)) //  may throw
            {
                try
                {
                    myDayModel->itemModels.append(_createWorkModel(work));  //  may throw
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log, but ignore
                    qCritical() << ex;
                }
            }
            for (const auto & event : account->events(_credentials, from, to)) //  may throw
            {
                try
                {
                    myDayModel->itemModels.append(_createEventModel(event));    //  may throw
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log, but ignore
                    qCritical() << ex;
                }
            }
            //  If there is a "current" activity add its item
            if (theCurrentActivity != nullptr)
            {
                try
                {
                    myDayModel->itemModels.append(_createCurrentActivityModel());   //  may throw
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log, but ignore
                    qCritical() << ex;
                }
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            myDayModel->itemModels.clear();
            //  Add a single "error" _ItemModel
            myDayModel->itemModels.append(
                std::make_shared<_ErrorModelImpl>(ex.errorMessage()));
        }
    }
    _breakLongWorks(myDayModel);
    _addDateIndicators(myDayModel);
    _sortChronologically(myDayModel);
    _breakWorksOnEvents(myDayModel);
    return myDayModel;
}

MyDayManager::_WorkModel MyDayManager::_createWorkModel(tt3::ws::Work work)
{
    tt3::ws::Activity activity = work->activity(_credentials);  //  may throw
    QString displayName = activity->displayName(_credentials);  //  may throw
    QString description = activity->description(_credentials).trimmed();    //  may throw
    QString tooltip =
        QLocale().toString(work->startedAt(_credentials).toLocalTime(), QLocale::FormatType::ShortFormat) +
        ".." +
        QLocale().toString(work->finishedAt(_credentials).toLocalTime(), QLocale::FormatType::ShortFormat) +
        "\n--------------------\n";
    tooltip +=
        description.isEmpty() ?
            displayName :
            displayName + "\n\n" + description;
    _WorkModel workModel =
        std::make_shared<_WorkModelImpl>(
            work,
            work->startedAt(_credentials).toLocalTime(),                //  may throw
            work->finishedAt(_credentials).toLocalTime(),               //  may throw
            displayName,
            activity->type()->smallIcon(),
            tooltip);
    return workModel;
}

MyDayManager::_EventModel MyDayManager::_createEventModel(tt3::ws::Event event)
{
    QString summary = event->summary(_credentials); //  may throw
    QString tooltip =
        QLocale().toString(event->occurredAt(_credentials).toLocalTime(), QLocale::FormatType::ShortFormat) +
        "\n--------------------\n";
    tooltip += summary;
    for (const auto & activity : event->activities(_credentials))
    {
        QString description = activity->description(_credentials).trimmed();    //  may throw
        if (!description.isEmpty())
        {
            tooltip += "\n\n";
            tooltip += description;
        }
    }
    _EventModel eventModel =
        std::make_shared<_EventModelImpl>(
            event,
            event->occurredAt(_credentials).toLocalTime(),  //  may throw
            summary,
            event->type()->smallIcon(),
            tooltip);
    return eventModel;
}

auto MyDayManager::_createCurrentActivityModel(
    ) -> _CurrentActivityModel
{
    QString displayName = theCurrentActivity->displayName(_credentials);  //  may throw
    QString description = theCurrentActivity->description(_credentials).trimmed();    //  may throw
    QString tooltip =
        description.isEmpty() ?
            displayName :
            displayName + "\n\n" + description;
    return std::make_shared<_CurrentActivityModelImpl>(
                theCurrentActivity,
                theCurrentActivity.lastChangedAt().toLocalTime(),
                displayName,
                theCurrentActivity->type()->smallIcon(),
                tooltip);
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
                _WorkModel head =
                    std::make_shared<_WorkModelImpl>(
                        workModel->work(),
                        headStartedAt,
                        headFinishedAt,
                        workModel->displayName(),
                        workModel->icon(),
                        workModel->tooltip());
                QDateTime tailStartedAt(startedOn.addDays(1), QTime(0, 0));
                QDateTime tailFinishedAt = workModel->finishedAt();
                _WorkModel tail =
                    std::make_shared<_WorkModelImpl>(
                        workModel->work(),
                        tailStartedAt,
                        tailFinishedAt,
                        workModel->displayName(),
                        workModel->icon(),
                        workModel->tooltip());
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
        QDate from = myDayModel->itemModels[i]->startedAt().date();
        QDate to = myDayModel->itemModels[i]->finishedAt().date();
        for (QDate d = from; d <= to; d = d.addDays(1))
        {
            datesWithWorks.insert(d);
        }
    }
    if (minDate == maxDate)
    {   //  We need at most one date indicator...
        if (minDate == today)
        {   //  ...which is optional
            return;
        }
        myDayModel->itemModels.append(
            std::make_shared<_DateModelImpl>(minDate));
        return;
    }
    Q_ASSERT(minDate < maxDate);
    for (QDate d = minDate; d <= maxDate; d = d.addDays(1))
    {
        if (datesWithWorks.contains(d))
        {
            myDayModel->itemModels.append(
                std::make_shared<_DateModelImpl>(d));
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
            if (a->startedAt() > b->startedAt())
            {
                return true;
            }
            else if (a->startedAt() == b->startedAt())
            {
                return a->finishedAt() > b->finishedAt();
            }
            else
            {
                return false;
            }
        });
}

void MyDayManager::_breakWorksOnEvents(_MyDayModel myDayModel)
{
    /*  TODO uncomment & fix assertion failure
#ifndef Q_NODEBUG
    for (int i = 0; i + 1 < myDayModel->itemModels.size(); i++)
    {
        Q_ASSERT(myDayModel->itemModels[i]->startedAt() <= myDayModel->itemModels[i + 1]->startedAt());
    }
#endif
    */

    for (qsizetype i = myDayModel->itemModels.size() - 1; i > 0; i--)
    {
        if (_WorkModel workModel =
            std::dynamic_pointer_cast<_WorkModelImpl>(myDayModel->itemModels[i]))
       {    //  A WorkModel...
            if (_EventModel eventModel =
                std::dynamic_pointer_cast<_EventModelImpl>(myDayModel->itemModels[i - 1]))
            {   //  ...is preceeded by EventModel...
                Q_ASSERT(eventModel->startedAt() == eventModel->finishedAt());
                if (eventModel->startedAt() > workModel->startedAt() &&
                    eventModel->startedAt() < workModel->finishedAt())
                {   //  ...which is in the middle of the WorkModel - split the
                    //  WorkModel into two around the event
                    QDateTime headStartedAt = workModel->startedAt();
                    QDateTime headFinishedAt = eventModel->startedAt();
                    _WorkModel head =
                        std::make_shared<_WorkModelImpl>(
                            workModel->work(),
                            headStartedAt,
                            headFinishedAt,
                            workModel->displayName(),
                            workModel->icon(),
                            workModel->tooltip());
                    QDateTime tailStartedAt = eventModel->finishedAt();
                    QDateTime tailFinishedAt = workModel->finishedAt();
                    _WorkModel tail =
                        std::make_shared<_WorkModelImpl>(
                            workModel->work(),
                            tailStartedAt,
                            tailFinishedAt,
                            workModel->displayName(),
                            workModel->icon(),
                            workModel->tooltip());
                    myDayModel->itemModels[i] = head;
                    myDayModel->itemModels.insert(i - 1, tail);
                    //  Skip over the new "tail" at [i] and keep going
                }
            }
        }
    }
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
    _ui->logListWidget->clear();
    _ui->logListWidget->setEnabled(false);
    _ui->logEventPushButton->setEnabled(false);
    _myDayModel->clear();
}

void MyDayManager::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MyDayManager));

    _ui->noQuickPicksLabel->setText(
        rr.string(RID(NoQuickPicksLabel)));
    _ui->quickPicksPushButton->setText(
        rr.string(RID(QuickPicksPushButton)));
    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->logEventPushButton->setText(
        rr.string(RID(LogEventPushButton)));

    //  [re]-populate the "filter" combo box
    int logDepth = _logDepth();
    _ui->filterComboBox->clear();
    _ui->filterComboBox->addItem(
        rr.string(RID(FilterDays1)),
        QVariant::fromValue(1));
    _ui->filterComboBox->addItem(
        rr.string(RID(FilterDays2)),
        QVariant::fromValue(2));
    _ui->filterComboBox->addItem(
        rr.string(RID(FilterDays3)),
        QVariant::fromValue(3));
    _ui->filterComboBox->addItem(
        rr.string(RID(FilterDays7)),
        QVariant::fromValue(7));
    _ui->filterComboBox->addItem(
        rr.string(RID(FilterDays30)),
        QVariant::fromValue(30));
    _setLogDepth(logDepth);
    refresh();
}

void MyDayManager::_recreateQuickPickButtons()
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MyDayManager));

    for (int i = 0; i < _quickPicksButtons.size(); i++)
    {
        delete _quickPicksButtons[i];
    }
    _quickPicksButtons.clear();
    //  Don't show "quick pick" buttons for completed tasks
    for (qsizetype i = _quickPicksList.size() - 1; i >= 0; i--)
    {
        if (auto task =
            std::dynamic_pointer_cast<tt3::ws::TaskImpl>(_quickPicksList[i]))
        {
            try
            {
                if (task->completed(_credentials))  //  may throw
                {
                    _quickPicksList.removeAt(i);
                }
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Log & suppress
                qCritical() << ex;
                _quickPicksList.removeAt(i);
            }
        }
    }
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
            QString suffix;
            if (tt3::ws::Task task =
                std::dynamic_pointer_cast<tt3::ws::TaskImpl>(_quickPicksList[i]))
            {
                if (task->completed(_credentials))
                {
                    suffix = "\n" + rr.string(RID(TaskCompletedSuffix));
                }
            }
            button = new QPushButton(
                _quickPicksList[i]->type()->smallIcon(),
                _quickPicksList[i]->displayName(_credentials) + suffix,   //  may throw
                this);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
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
        button->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        connect(button,
                &QWidget::customContextMenuRequested,
                this,
                &MyDayManager::_quickPickButtonCustomContextMenuRequested);
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
            qCritical() << ex;
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
        _ui->logListWidget->item(i)->setFont(
            _myDayModel->itemModels[i]->isEmphasized() ?
                _listWidgetDecorations.itemEmphasisFont :
                _listWidgetDecorations.itemFont);
        _ui->logListWidget->item(i)->setToolTip(_myDayModel->itemModels[i]->tooltip());
    }
}

int MyDayManager::_logDepth()
{
    return _ui->filterComboBox->currentData().value<int>();
}

void MyDayManager::_setLogDepth(int logDepth)
{
    for (int i = 0; i < _ui->filterComboBox->count(); i++)
    {
        if (_ui->filterComboBox->itemData(i).value<int>() == logDepth)
        {   //  This one!
            _ui->filterComboBox->setCurrentIndex(i);
            break;
        }
    }
}


//////////
//  Signal handlers
void MyDayManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _pushButtonDecorations = PushButtonDecorations(_ui->quickPicksPushButton);
    _listWidgetDecorations = ListWidgetDecorations(_ui->logListWidget);
    requestRefresh();
}

void MyDayManager::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
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
        tt3::ws::Account account = _workspace->login(_credentials);//   may throw
        ManageQuickPicksListDialog dlg(this, account, _credentials);
        if (dlg.doModal() == ManageQuickPicksListDialog::Result::Ok)
        {   //  Refresh quick picks buttons
            account->setQuickPicksList(_credentials, dlg.quickPicksList());
            _recreateDynamicControls();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void MyDayManager::_logEventPushButtonClicked()
{
    EnterCommentDialog dlg(this, _workspace);
    if (dlg.doModal() == EnterCommentDialog::Result::Ok)
    {
        try
        {
            tt3::ws::Account callerAccount =
                _workspace->login(_credentials);    //  may throw
            callerAccount->createEvent( //  may throw
                _credentials,
                QDateTime::currentDateTimeUtc(),
                dlg.comment(),
                tt3::ws::Activities());
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & suppress
            qCritical() << ex;
        }
    }
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

void MyDayManager::_filterComboBoxCurrentIndexChanged(int)
{
    if (_constructed)
    {
        Component::Settings::instance()->myDayLogDepth = _logDepth();
        _myDayModel = _createMyDayModel();
        requestRefresh();
    }
}

void MyDayManager::_viewOptionSettingValueChanged()
{
    _setLogDepth(Component::Settings::instance()->myDayLogDepth);
    _myDayModel = _createMyDayModel();
    requestRefresh();
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

void MyDayManager::_logListWidgetCustomContextMenuRequested(QPoint p)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MyDayManager));

    if (auto item = _ui->logListWidget->itemAt(p))
    {
        int n = _ui->logListWidget->row(item);
        Q_ASSERT(n >= 0 && n < _myDayModel->itemModels.size());
        _ui->logListWidget->setCurrentRow(n);
        //  Re-create context menu
        _contextMenu.reset(new QMenu());
        QAction * destroyObjectAction = nullptr;
        if (auto workModel =
            std::dynamic_pointer_cast<_WorkModelImpl>(_myDayModel->itemModels[n]))
        {
            _contextMenuObject = workModel->work();
            destroyObjectAction =
                _contextMenu->addAction(
                    QIcon(":/tt3-gui/Resources/Images/Actions/DestroyWorkSmall.png"),
                    rr.string(RID(DestroyWorkAction)));
        }
        else if (auto eventModel =
                 std::dynamic_pointer_cast<_EventModelImpl>(_myDayModel->itemModels[n]))
        {
            _contextMenuObject = eventModel->event();
            destroyObjectAction =
                _contextMenu->addAction(
                    QIcon(":/tt3-gui/Resources/Images/Actions/DestroyEventSmall.png"),
                    rr.string(RID(DestroyEventAction)));
        }
        else
        {   //  No context menu
            return;
        }
        //  Adjust menu item states
        try
        {
            destroyObjectAction->setEnabled(
                !_workspace->isReadOnly() &&
                _contextMenuObject->canDestroy(_credentials));
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            destroyObjectAction->setEnabled(false);
        }
        //  Set up signal handling
        connect(destroyObjectAction,
                &QAction::triggered,
                this,
                &MyDayManager::_destroyObjectContextActionTriggered);
        //  Go!
        _contextMenu->popup(_ui->logListWidget->mapToGlobal(p));
    }
}

void MyDayManager::_quickPickButtonCustomContextMenuRequested(QPoint p)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MyDayManager));

    if (auto pushButton = dynamic_cast<QPushButton*>(sender()))
    {
        qsizetype n = _quickPicksButtons.indexOf(pushButton);
        if (n >= 0 && n < _quickPicksButtons.size())
        {   //  Re-create context menu
            _contextMenu.reset(new QMenu());
            QAction * modifyObjectAction = nullptr;
            if ((_contextMenuObject =
                 std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(_quickPicksList[n])))
            {
                try
                {
                    if (_contextMenuObject->canModify(_credentials))    //  may throw
                    {
                        modifyObjectAction =
                            _contextMenu->addAction(
                                QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPublicTaskSmall.png"),
                                rr.string(RID(ModifyPublicTaskAction)));
                    }
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & tret as view-only
                    qCritical() << ex;
                }
                if (modifyObjectAction == nullptr)
                {
                    modifyObjectAction =
                        _contextMenu->addAction(
                            QIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicTaskSmall.png"),
                            rr.string(RID(ViewPublicTaskAction)));
                }
            }
            else if ((_contextMenuObject =
                        std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(_quickPicksList[n])))
            {
                try
                {
                    if (_contextMenuObject->canModify(_credentials))    //  may throw
                    {
                        modifyObjectAction =
                            _contextMenu->addAction(
                                QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPrivateTaskSmall.png"),
                                rr.string(RID(ModifyPrivateTaskAction)));
                    }
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & tret as view-only
                    qCritical() << ex;
                }
                if (modifyObjectAction == nullptr)
                {
                    modifyObjectAction =
                        _contextMenu->addAction(
                            QIcon(":/tt3-gui/Resources/Images/Actions/ViewPrivateTaskSmall.png"),
                            rr.string(RID(ViewPrivateTaskAction)));
                }
            }
            else if ((_contextMenuObject =
                        std::dynamic_pointer_cast<tt3::ws::PublicActivityImpl>(_quickPicksList[n])))
            {
                try
                {
                    if (_contextMenuObject->canModify(_credentials))    //  may throw
                    {
                        modifyObjectAction =
                            _contextMenu->addAction(
                                QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPublicActivitySmall.png"),
                                rr.string(RID(ModifyPublicActivityAction)));
                    }
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & tret as view-only
                    qCritical() << ex;
                }
                if (modifyObjectAction == nullptr)
                {
                    modifyObjectAction =
                        _contextMenu->addAction(
                            QIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicActivitySmall.png"),
                            rr.string(RID(ViewPublicActivityAction)));
                }
            }
            else if ((_contextMenuObject =
                        std::dynamic_pointer_cast<tt3::ws::PrivateActivityImpl>(_quickPicksList[n])))
            {
                try
                {
                    if (_contextMenuObject->canModify(_credentials))    //  may throw
                    {
                        modifyObjectAction =
                            _contextMenu->addAction(
                                QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPrivateActivitySmall.png"),
                                rr.string(RID(ModifyPrivateActivityAction)));
                    }
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & tret as view-only
                    qCritical() << ex;
                }
                if (modifyObjectAction == nullptr)
                {
                    modifyObjectAction =
                        _contextMenu->addAction(
                            QIcon(":/tt3-gui/Resources/Images/Actions/ViewPrivateActivitySmall.png"),
                            rr.string(RID(ViewPrivateActivityAction)));
                }
            }
            else
            {   //  No context menu
                return;
            }
            _contextMenu->addSeparator();
            QAction * removeObjectAction =
                _contextMenu->addAction(
                    QIcon(":/tt3-gui/Resources/Images/Actions/RemoveSmall.png"),
                    rr.string(RID(RemoveFromQuickPicksAction)));
            //  Adjust menu item states
            removeObjectAction->setEnabled(!_workspace->isReadOnly());
            //  Set up signal handling
            connect(modifyObjectAction,
                    &QAction::triggered,
                    this,
                    &MyDayManager::_modifyObjectContextActionTriggered);
            connect(removeObjectAction,
                    &QAction::triggered,
                    this,
                    &MyDayManager::_removeActivityFromQuickPicksContextActionTriggered);
            //  Go!
            _contextMenu->popup(pushButton->mapToGlobal(p));
        }
    }
}

void MyDayManager::_modifyObjectContextActionTriggered()
{
    if (auto publicTask =
        std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(_contextMenuObject))
    {
        try
        {
            ModifyPublicTaskDialog dlg(this, publicTask, _credentials); //  may throw
            if (dlg.doModal() == ModifyPublicTaskDialog::Result::Ok)
            {
                _recreateDynamicControls();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
    }
    else if (auto privateTask =
             std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(_contextMenuObject))
    {
        try
        {
            ModifyPrivateTaskDialog dlg(this, privateTask, _credentials); //  may throw
            if (dlg.doModal() == ModifyPrivateTaskDialog::Result::Ok)
            {
                _recreateDynamicControls();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
    }
    else if (auto publicActivity =
             std::dynamic_pointer_cast<tt3::ws::PublicActivityImpl>(_contextMenuObject))
    {
        try
        {
            ModifyPublicActivityDialog dlg(this, publicActivity, _credentials); //  may throw
            if (dlg.doModal() == ModifyPublicActivityDialog::Result::Ok)
            {
                _recreateDynamicControls();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
    }
    else if (auto privateActivity =
             std::dynamic_pointer_cast<tt3::ws::PrivateActivityImpl>(_contextMenuObject))
    {
        try
        {
            ModifyPrivateActivityDialog dlg(this, privateActivity, _credentials); //  may throw
            if (dlg.doModal() == ModifyPrivateActivityDialog::Result::Ok)
            {
                _recreateDynamicControls();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
    }
}

void MyDayManager::_removeActivityFromQuickPicksContextActionTriggered()
{
    if (auto activity =
        std::dynamic_pointer_cast<tt3::ws::ActivityImpl>(_contextMenuObject))
    {
        try
        {
            _quickPicksList.removeOne(activity);
            tt3::ws::Account account = _workspace->login(_credentials); //  may throw
            account->setQuickPicksList(_credentials, _quickPicksList);  //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
        _recreateDynamicControls();
    }
}

void MyDayManager::_destroyObjectContextActionTriggered()
{
    if (auto work =
        std::dynamic_pointer_cast<tt3::ws::WorkImpl>(_contextMenuObject))
    {
        try
        {
            DestroyWorkDialog dlg(this, work, _credentials);
            if (dlg.doModal() == DestroyWorkDialog::Result::Ok)
            {
                _myDayModel = _createMyDayModel();
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            _myDayModel = _createMyDayModel();
            requestRefresh();
        }
    }
    else if (auto event =
             std::dynamic_pointer_cast<tt3::ws::EventImpl>(_contextMenuObject))
    {
        try
        {
            DestroyEventDialog dlg(this, event, _credentials);
            if (dlg.doModal() == DestroyEventDialog::Result::Ok)
            {
                _myDayModel = _createMyDayModel();
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            _myDayModel = _createMyDayModel();
            requestRefresh();
        }
    }
}

//  End of tt3-gui/MyDayManager.cpp
