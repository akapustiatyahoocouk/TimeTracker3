//
//  tt3-gui/UserManager.cpp - tt3::gui::UserManager class implementation
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
#include "ui_UserManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
UserManager::UserManager(QWidget * parent)
    :   QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::UserManager)
{
    _ui->setupUi(this);
    _decorations = TreeWidgetDecorations(_ui->usersTreeWidget);
    _applyCurrentLocale();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &UserManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &UserManager::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  View options changes should cause a refresh
    connect(&Component::Settings::instance()->showDisabledUsersAndAccounts,
            &tt3::util::AbstractSetting::valueChanged,
            this,
            &UserManager::_viewOptionSettingValueChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &UserManager::refreshRequested,
            this,
            &UserManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();
}

UserManager::~UserManager()
{
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace UserManager::workspace() const
{
    return _workspace;
}

void UserManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials UserManager::credentials() const
{
    return _credentials;
}

void UserManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
       requestRefresh();
    }
}

void UserManager::refresh()
{
    static const QIcon viewUserIcon(":/tt3-gui/Resources/Images/Actions/ViewUserLarge.png");
    static const QIcon modifyUserIcon(":/tt3-gui/Resources/Images/Actions/ModifyUserLarge.png");
    static const QIcon viewAccountIcon(":/tt3-gui/Resources/Images/Actions/ViewAccountLarge.png");
    static const QIcon modifyAccountIcon(":/tt3-gui/Resources/Images/Actions/ModifyAccountLarge.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(UserManager));

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
        _ui->filterLabel->setEnabled(true);
        _ui->filterLineEdit->setEnabled(true);
        _ui->usersTreeWidget->setEnabled(true);
        _ui->showDisabledCheckBox->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel = _createWorkspaceModel();
        if (!Component::Settings::instance()->showDisabledUsersAndAccounts)
        {
            _removeDisabledItems(workspaceModel);
        }
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {
            _filterItems(workspaceModel);
        }
        _refreshWorkspaceTree(workspaceModel);
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->usersTreeWidget->expandAll();
        }

        tt3::ws::User currentUser = _currentUser();
        tt3::ws::Account currentAccount = _currentAccount();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createUserPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                    tt3::ws::Capability::ManageUsers));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->createUserPushButton->setEnabled(false);
        }
        _ui->modifyUserPushButton->setEnabled(
            currentUser != nullptr);
        try
        {
            _ui->destroyUserPushButton->setEnabled(
                !readOnly &&
                currentUser != nullptr &&
                currentUser->canDestroy(_credentials));    //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->destroyUserPushButton->setEnabled(false);
        }
        try
        {
            _ui->createAccountPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(
                    _credentials,
                    tt3::ws::Capability::Administrator |
                    tt3::ws::Capability::ManageUsers) &&
                currentUser != nullptr);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->createAccountPushButton->setEnabled(false);
        }
        _ui->modifyAccountPushButton->setEnabled(
            currentAccount != nullptr);
        try
        {
            _ui->destroyAccountPushButton->setEnabled(
                !readOnly &&
                currentAccount != nullptr &&
                currentAccount->canDestroy(_credentials)); //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->destroyAccountPushButton->setEnabled(false);
        }

        _ui->showDisabledCheckBox->setChecked(
            Component::Settings::instance()->showDisabledUsersAndAccounts);

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (currentUser != nullptr &&
                !currentUser->workspace()->isReadOnly() &&
                currentUser->canModify(_credentials))  //  may throw
            {   //  RW
                _ui->modifyUserPushButton->setIcon(modifyUserIcon);
                _ui->modifyUserPushButton->setText(
                    rr.string(RID(ModifyUserPushButton)));
            }
            else
            {   //  RO
                _ui->modifyUserPushButton->setIcon(viewUserIcon);
                _ui->modifyUserPushButton->setText(
                    rr.string(RID(ViewUserPushButton)));
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex;
            _ui->modifyUserPushButton->setIcon(viewUserIcon);
            _ui->modifyUserPushButton->setText(
                rr.string(RID(ViewUserPushButton)));
        }
        try
        {
            if (currentAccount != nullptr &&
                !currentAccount->workspace()->isReadOnly() &&
                currentAccount->canModify(_credentials))   //  may throw
            {   //  RW
                _ui->modifyAccountPushButton->setIcon(modifyAccountIcon);
                _ui->modifyAccountPushButton->setText(
                    rr.string(RID(ModifyAccountPushButton)));
            }
            else
            {   //  RO
                _ui->modifyAccountPushButton->setIcon(viewAccountIcon);
                _ui->modifyAccountPushButton->setText(
                    rr.string(RID(ViewAccountPushButton)));
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex;
            _ui->modifyAccountPushButton->setIcon(viewAccountIcon);
            _ui->modifyAccountPushButton->setText(
                rr.string(RID(ViewAccountPushButton)));
        }
    }
}

void UserManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
UserManager::_WorkspaceModel UserManager::_createWorkspaceModel()
{
    auto workspaceModel = std::make_shared<_WorkspaceModelImpl>();
    try
    {
        for (tt3::ws::User user : _workspace->users(_credentials))    //  may throw
        {
            workspaceModel->userModels.append(_createUserModel(user));
        }
        std::sort(
            workspaceModel->userModels.begin(),
            workspaceModel->userModels.end(),
            [&](auto a, auto b)
            { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        workspaceModel->userModels.clear();
    }
    return workspaceModel;
}

UserManager::_UserModel UserManager::_createUserModel(tt3::ws::User user)
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(UserManager));

    auto userModel = std::make_shared<_UserModelImpl>(user);
    try
    {
        userModel->text = user->realName(_credentials);
        if (!user->enabled(_credentials))
        {
            userModel->text += " " + rr.string(RID(UserDisabledSuffix));
            userModel->brush = _decorations.disabledItemForeground;
        }
        else
        {
            userModel->brush = _decorations.itemForeground;
        }
        userModel->icon = user->type()->smallIcon();
        userModel->font = _decorations.itemFont;
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        userModel->text = ex.errorMessage();
        userModel->icon = errorIcon;
        userModel->font = _decorations.itemFont;
        userModel->brush = _decorations.errorItemForeground;
    }
    try
    {
        for (tt3::ws::Account account : user->accounts(_credentials))    //  may throw
        {
            userModel->accountModels.append(_createAccountModel(account));
        }
        std::sort(
            userModel->accountModels.begin(),
            userModel->accountModels.end(),
            [&](auto a, auto b)
            { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        userModel->accountModels.clear();
    }
    return userModel;
}

UserManager::_AccountModel UserManager::_createAccountModel(tt3::ws::Account account)
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(UserManager));

    auto accountModel = std::make_shared<_AccountModelImpl>(account);
    try
    {
        accountModel->text = account->login(_credentials);
        if (!account->enabled(_credentials))
        {
            accountModel->text += " " + rr.string(RID(AccountDisabledSuffix));
            accountModel->brush = _decorations.disabledItemForeground;
        }
        else
        {
            accountModel->brush = _decorations.itemForeground;
        }
        accountModel->icon = account->type()->smallIcon();
        accountModel->font = _decorations.itemFont;
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        accountModel->text = ex.errorMessage();
        accountModel->icon = errorIcon;
        accountModel->font = _decorations.itemFont;
        accountModel->brush = _decorations.errorItemForeground;
    }
    return accountModel;
}

void UserManager::_removeDisabledItems(_WorkspaceModel workspaceModel)
{
    for (qsizetype i = workspaceModel->userModels.size() - 1; i >= 0; i--)
    {
        _UserModel userModel = workspaceModel->userModels[i];
        _removeDisabledItems(userModel);
        //  If this user is disabled...
        try
        {
            if (!userModel->user->enabled(_credentials))    //  may throw
            {
                if (userModel->accountModels.isEmpty())
                {   //  ...and has no accounts models - delete it
                    workspaceModel->userModels.removeAt(i);
                }
                //  ...else it has account models - keep it
            }
            //  ...else keep this user item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void UserManager::_removeDisabledItems(_UserModel userModel)
{
    for (qsizetype i = userModel->accountModels.size() - 1; i >= 0; i--)
    {
        try
        {
            if (!userModel->accountModels[i]->account->enabled(_credentials))   //  may throw
            {   //  We don't want this account item
                userModel->accountModels.removeAt(i);
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void UserManager::_filterItems(_WorkspaceModel workspaceModel)
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->userModels.size() - 1; i >= 0; i--)
    {
        _UserModel userModel = workspaceModel->userModels[i];
        _filterItems(userModel);
        if (userModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            userModel->brush = _decorations.filterMatchItemForeground;
        }
        else if (userModel->accountModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            workspaceModel->userModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            workspaceModel->userModels[i]->brush = _decorations.disabledItemForeground;
        }
    }
}

void UserManager::_filterItems(_UserModel userModel)
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = userModel->accountModels.size() - 1; i >= 0; i--)
    {
        try
        {
            _AccountModel accountModel = userModel->accountModels[i];
            if (accountModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
            {   //  Item matches the filter - mark it as a match
                accountModel->brush = _decorations.filterMatchItemForeground;
            }
            else
            {   //  Item does not match he filter - remove it
                userModel->accountModels.removeAt(i);
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void UserManager::_refreshWorkspaceTree(
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Make sure the "users" tree contains
    //  a proper number of root (User) items...
    while (_ui->usersTreeWidget->topLevelItemCount() < workspaceModel->userModels.size())
    {   //  Too few root (User) items
        _ui->usersTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (_ui->usersTreeWidget->topLevelItemCount() > workspaceModel->userModels.size())
    {   //  Too many root (User) items
        delete _ui->usersTreeWidget->takeTopLevelItem(
            _ui->usersTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper User and has proper children
    for (int i = 0; i < workspaceModel->userModels.size(); i++)
    {
        _refreshUserItem(
            _ui->usersTreeWidget->topLevelItem(i),
            workspaceModel->userModels[i]);
    }
}

void UserManager::_refreshUserItem(
        QTreeWidgetItem * userItem,
        _UserModel userModel)
{
    Q_ASSERT(userItem != nullptr);
    Q_ASSERT(userModel != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Refresh User item properties
    userItem->setText(0, userModel->text);
    userItem->setIcon(0, userModel->icon);
    userItem->setForeground(0, userModel->brush);
    userItem->setFont(0, userModel->font);
    userItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(userModel->user));
    //  Make sure the "users" item contains
    //  a proper number of child (Account) items...
    while (userItem->childCount() < userModel->accountModels.size())
    {   //  Too few leaf (Account) items
        userItem->addChild(new QTreeWidgetItem());
    }
    while (userItem->childCount() > userModel->accountModels.size())
    {   //  Too many leaf (Account) items
        delete userItem->takeChild(userItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper Account
    for (int i = 0; i < userModel->accountModels.size(); i++)
    {
        _refreshAccountItem(
            userItem->child(i),
            userModel->accountModels[i]);
    }
}

void UserManager::_refreshAccountItem(
        QTreeWidgetItem * accountItem,
        _AccountModel accountModel
    )
{
    Q_ASSERT(accountItem != nullptr);
    Q_ASSERT(accountModel != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Refresh Account item properties
    accountItem->setText(0, accountModel->text);
    accountItem->setIcon(0, accountModel->icon);
    accountItem->setForeground(0, accountModel->brush);
    accountItem->setFont(0, accountModel->font);
    accountItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(accountModel->account));
    //  There will be no further children
    Q_ASSERT(accountItem->childCount() == 0);
}

//////////
//  Implementation helpers
tt3::ws::User UserManager::_currentUser()
{
    QTreeWidgetItem * item = _ui->usersTreeWidget->currentItem();
    return (item != nullptr && item->parent() == nullptr) ?
                item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>() :
                nullptr;
}

void UserManager::_setCurrentUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->usersTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * userItem = _ui->usersTreeWidget->topLevelItem(i);
        if (user == userItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>())
        {   //  This one!
            _ui->usersTreeWidget->setCurrentItem(userItem);
            return;
        }
    }
}

tt3::ws::Account UserManager::_currentAccount()
{
    QTreeWidgetItem * item = _ui->usersTreeWidget->currentItem();
    return (item != nullptr && item->parent() != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Account>() :
               nullptr;
}

void UserManager::_setCurrentAccount(tt3::ws::Account account)
{
    for (int i = 0; i < _ui->usersTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * userItem = _ui->usersTreeWidget->topLevelItem(i);
        for (int j = 0; j < userItem->childCount(); j++)
        {
            QTreeWidgetItem * accountItem = userItem->child(j);
            if (account == accountItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Account>())
            {   //  This one!
                _ui->usersTreeWidget->setCurrentItem(accountItem);
                return;
            }
        }
    }
}

void UserManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &UserManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &UserManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &UserManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &UserManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void UserManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &UserManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &UserManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &UserManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &UserManager::_objectModified);
    }
}

void UserManager::_clearAndDisableAllControls()
{
    _ui->usersTreeWidget->clear();
    _ui->filterLineEdit->setText("");
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->usersTreeWidget->setEnabled(false);
    _ui->createUserPushButton->setEnabled(false);
    _ui->modifyUserPushButton->setEnabled(false);
    _ui->destroyUserPushButton->setEnabled(false);
    _ui->createAccountPushButton->setEnabled(false);
    _ui->modifyAccountPushButton->setEnabled(false);
    _ui->destroyAccountPushButton->setEnabled(false);
    _ui->showDisabledCheckBox->setEnabled(false);
}

void UserManager::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(UserManager));

    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->createUserPushButton->setText(
        rr.string(RID(CreateUserPushButton)));
    _ui->modifyUserPushButton->setText(
        rr.string(RID(ModifyUserPushButton)));
    _ui->destroyUserPushButton->setText(
        rr.string(RID(DestroyUserPushButton)));
    _ui->createAccountPushButton->setText(
        rr.string(RID(CreateAccountPushButton)));
    _ui->modifyAccountPushButton->setText(
        rr.string(RID(ModifyAccountPushButton)));
    _ui->destroyAccountPushButton->setText(
        rr.string(RID(DestroyAccountPushButton)));
    _ui->showDisabledCheckBox->setText(
        rr.string(RID(ShowDisabledCheckBox)));
    refresh();
}

//////////
//  Signal handlers
void UserManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->usersTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->usersTreeWidget);
   requestRefresh();
}

void UserManager::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
}

void UserManager::_usersTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
   requestRefresh();
}

void UserManager::_usersTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _usersTreeContextMenu.reset(new QMenu());
    QAction * createUserAction =
        _usersTreeContextMenu->addAction(
            _ui->createUserPushButton->icon(),
            _ui->createUserPushButton->text());
    QAction * modifyUserAction =
        _usersTreeContextMenu->addAction(
            _ui->modifyUserPushButton->icon(),
            _ui->modifyUserPushButton->text());
    QAction * destroyUserAction =
        _usersTreeContextMenu->addAction(
            _ui->destroyUserPushButton->icon(),
            _ui->destroyUserPushButton->text());
    _usersTreeContextMenu->addSeparator();
    QAction * createAccountAction =
        _usersTreeContextMenu->addAction(
            _ui->createAccountPushButton->icon(),
            _ui->createAccountPushButton->text());
    QAction * modifyAccountAction =
        _usersTreeContextMenu->addAction(
            _ui->modifyAccountPushButton->icon(),
            _ui->modifyAccountPushButton->text());
    QAction * destroyAccountAction =
        _usersTreeContextMenu->addAction(
            _ui->destroyAccountPushButton->icon(),
            _ui->destroyAccountPushButton->text());
    //  Adjust menu item states
    createUserAction->setEnabled(_ui->createUserPushButton->isEnabled());
    modifyUserAction->setEnabled(_ui->modifyUserPushButton->isEnabled());
    destroyUserAction->setEnabled(_ui->destroyUserPushButton->isEnabled());
    createAccountAction->setEnabled(_ui->createAccountPushButton->isEnabled());
    modifyAccountAction->setEnabled(_ui->modifyAccountPushButton->isEnabled());
    destroyAccountAction->setEnabled(_ui->destroyAccountPushButton->isEnabled());
    //  Set up signal handling
    connect(createUserAction,
            &QAction::triggered,
            this,
            &UserManager::_createUserPushButtonClicked);
    connect(modifyUserAction,
            &QAction::triggered,
            this,
            &UserManager::_modifyUserPushButtonClicked);
    connect(destroyUserAction,
            &QAction::triggered,
            this,
            &UserManager::_destroyUserPushButtonClicked);
    connect(createAccountAction,
            &QAction::triggered,
            this,
            &UserManager::_createAccountPushButtonClicked);
    connect(modifyAccountAction,
            &QAction::triggered,
            this,
            &UserManager::_modifyAccountPushButtonClicked);
    connect(destroyAccountAction,
            &QAction::triggered,
            this,
            &UserManager::_destroyAccountPushButtonClicked);
    //  Go!
    _usersTreeContextMenu->popup(_ui->usersTreeWidget->mapToGlobal(p));
}

void UserManager::_createUserPushButtonClicked()
{
    try
    {
        CreateUserDialog dlg(this, _workspace, _credentials);   //  may throw
        if (dlg.doModal() == CreateUserDialog::Result::Ok)
        {   //  User created
            refresh();  //  must refresh NOW
            _setCurrentUser(dlg.createdUser());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void UserManager::_modifyUserPushButtonClicked()
{
    if (auto user = _currentUser())
    {
        try
        {
            ModifyUserDialog dlg(this, user, _credentials); //  may throw
            if (dlg.doModal() == ModifyUserDialog::Result::Ok)
            {   //  User modified - its position in the users tree may have changed
                refresh();  //  must refresh NOW
                _setCurrentUser(user);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void UserManager::_destroyUserPushButtonClicked()
{
    if (auto user = _currentUser())
    {
        try
        {
            DestroyUserDialog dlg(this, user, _credentials); //  may throw
            if (dlg.doModal() == DestroyUserDialog::Result::Ok)
            {   //  User destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void UserManager::_createAccountPushButtonClicked()
{
    if (auto user = _currentUser())
    {
        try
        {
            CreateAccountDialog dlg(this, user, _credentials);  //  may throw
            if (dlg.doModal() == CreateAccountDialog::Result::Ok)
            {   //  Account created
                refresh();  //  must refresh NOW
                _setCurrentAccount(dlg.createdAccount());
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            tt3::gui::ErrorDialog::show(this, ex);
        }
    }
}

void UserManager::_modifyAccountPushButtonClicked()
{
    if (auto account = _currentAccount())
    {
        try
        {
            ModifyAccountDialog dlg(this, account, _credentials); //  may throw
            if (dlg.doModal() == ModifyAccountDialog::Result::Ok)
            {   //  User modified - its position in the users tree may have changed
                refresh();  //  must refresh NOW
                _setCurrentAccount(account);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void UserManager::_destroyAccountPushButtonClicked()
{
    if (auto account = _currentAccount())
    {
        try
        {
            DestroyAccountDialog dlg(this, account, _credentials); //  may throw
            if (dlg.doModal() == DestroyAccountDialog::Result::Ok)
            {   //  Destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void UserManager::_showDisabledCheckBoxToggled(bool)
{
    Component::Settings::instance()->showDisabledUsersAndAccounts =
        _ui->showDisabledCheckBox->isChecked();
   requestRefresh();
}

void UserManager::_viewOptionSettingValueChanged()
{
   requestRefresh();
}

void UserManager::_filterLineEditTextChanged(QString)
{
   requestRefresh();
}

void UserManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
   requestRefresh();
}

void UserManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
   requestRefresh();
}

void UserManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
   requestRefresh();
}

void UserManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
   requestRefresh();
}

void UserManager::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/UserManager.cpp
