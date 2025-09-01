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

//////////
//  Construction/destruction
UserManager::UserManager(QWidget * parent)
    :   QWidget(parent),
        //  Implementation
        _workspace(tt3::ws::theCurrentWorkspace),
        _credentials(tt3::ws::theCurrentCredentials),
        //  Controls
        _ui(new Ui::UserManager)
{
    _ui->setupUi(this);

    //  TODO start listening for change notifications
    //  on the currently "viewed" Workspace
}

UserManager::~UserManager()
{
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
        _workspace = workspace;
        refresh();
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
        refresh();
    }
}

void UserManager::refresh()
{
    if (_workspace == nullptr || !_credentials.isValid() ||
        !_workspace->canAccess(_credentials)) //  TODO handle WorkspaceExceptions
    {   //  Nothing to show...
        _ui->usersTreeWidget->clear();
        //  ...so disable all controls...
        _ui->filterLabel->setEnabled(false);
        _ui->filterLineEdit->setEnabled(false);
        _ui->usersTreeWidget->setEnabled(false);
        _ui->createUserPushButton->setEnabled(false);
        _ui->modifyUserPushButton->setEnabled(false);
        _ui->destroyUserPushButton->setEnabled(false);
        _ui->createAccountPushButton->setEnabled(false);
        _ui->modifyAccountPushButton->setEnabled(false);
        _ui->destroyAccountPushButton->setEnabled(false);
        //  ...and we're done
        return;
    }

    //  Otherwise some controls are always enabled...
    _ui->filterLabel->setEnabled(true);
    _ui->filterLineEdit->setEnabled(true);
    _ui->usersTreeWidget->setEnabled(true);

    //  ...while others are enabled based on current
    //  selection and permissions granted by Credentials
    _refreshUserItems();

    tt3::ws::Capabilities capabilities;
    try
    {
        capabilities = _workspace->capabilities(_credentials);  //  may throw
    }
    catch (const tt3::util::Exception &)
    {   //  OOPS! Suppress, though
        capabilities = tt3::ws::Capabilities::None;
    }

    tt3::ws::User selectedUser = _selectedUser();
    tt3::ws::Account selectedAccount = _selectedAccount();
    _ui->createUserPushButton->setEnabled(
        (capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None ||
        (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None);
    _ui->modifyUserPushButton->setEnabled(
        ((capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None ||
         (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None) &&
        selectedUser != nullptr);
    _ui->destroyUserPushButton->setEnabled(
        ((capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None ||
         (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None) &&
        selectedUser != nullptr);
    _ui->createAccountPushButton->setEnabled(
        ((capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None ||
         (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None) &&
        selectedUser != nullptr);
    _ui->modifyAccountPushButton->setEnabled(
        ((capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None ||
         (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None) &&
        selectedAccount != nullptr);
    _ui->destroyAccountPushButton->setEnabled(
        ((capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None ||
         (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None) &&
        selectedAccount != nullptr);
}

//////////
//  Implementation helpers
void UserManager::_refreshUserItems()
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Which Users currently exist (sorted by real name) ?
    QList<tt3::ws::User> users;
    try
    {
        users = _workspace->users(_credentials).values();    //  may throw
    }
    catch (...)
    {   //  OOPS! Suppress, though
    }
    try
    {
        std::sort(users.begin(),
                  users.end(),
                  [&](auto a, auto b)
                    { return a->realName(_credentials) < b->realName(_credentials); });   //  may throw
    }
    catch (...)
    {   //  OOPS! Sort by OID to ensure a stable order
        std::sort(users.begin(),
                  users.end(),
                  [](auto a, auto b)
                    { return a->oid() < b->oid(); });
    }

    //  Make sure the "users" tree contains
    //  a proper number of root (User) items...
    while (_ui->usersTreeWidget->topLevelItemCount() < users.size())
    {   //  Too few root (User) items
        tt3::ws::User user = users[_ui->usersTreeWidget->topLevelItemCount()];
        QTreeWidgetItem * userItem = new QTreeWidgetItem();
        try
        {
            userItem->setText(0, _userItemText(user));      //  may throw
            userItem->setIcon(0, user->type()->smallIcon());//  nothrow
        }
        catch (const tt3::util::Exception & ex)
        {
            userItem->setText(0, ex.errorMessage());
            userItem->setIcon(0, errorIcon);
        }
        _ui->usersTreeWidget->addTopLevelItem(userItem);
        userItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(user));
    }
    while (_ui->usersTreeWidget->topLevelItemCount() > users.size())
    {   //  Too many root (User) items
        delete _ui->usersTreeWidget->takeTopLevelItem(
            _ui->usersTreeWidget->topLevelItemCount() - 1);
    }

    //  ...and that each top-level item represents
    //  a proper User and has proper children
    for (int i = 0; i < users.size(); i++)
    {
        QTreeWidgetItem * userItem = _ui->usersTreeWidget->topLevelItem(i);
        tt3::ws::User user = userItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>();
        if (user != users[i])
        {   //  Replace tree item data...
            user = users[i];
            userItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(user));
        }
        //  ...adjust its text/icon...
        try
        {
            userItem->setText(0, _userItemText(user));      //  may throw
            userItem->setIcon(0, user->type()->smallIcon());//  nothrow
        }
        catch (const tt3::util::Exception & ex)
        {
            userItem->setText(0, ex.errorMessage());
            userItem->setIcon(0, errorIcon);
        }
        //  ...and children
        _refreshAccountItems(userItem);
    }
}

void UserManager::_refreshAccountItems(QTreeWidgetItem * userItem)
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    Q_ASSERT(userItem != nullptr);
    Q_ASSERT(_credentials.isValid());
    tt3::ws::User user = userItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>();

    //  Which Accounts currently exist (sorted by login) ?
    QList<tt3::ws::Account> accounts;
    try
    {
        accounts = user->accounts(_credentials).values();    //  may throw
    }
    catch (...)
    {   //  OOPS! Suppress, though
    }
    try
    {
        std::sort(accounts.begin(),
                  accounts.end(),
                  [&](auto a, auto b)
                  { return a->login(_credentials) < b->login(_credentials); });   //  may throw
    }
    catch (...)
    {   //  OOPS! Sort by OID to ensure a stable order
        std::sort(accounts.begin(),
                  accounts.end(),
                  [](auto a, auto b)
                  { return a->oid() < b->oid(); });
    }

    //  Make sure the "users" tree contains
    //  a proper number of leaf (Account) items...
    while (userItem->childCount() < accounts.size())
    {   //  Too few leaf (Account) items
        tt3::ws::Account account = accounts[userItem->childCount()];
        QTreeWidgetItem * accountItem = new QTreeWidgetItem();
        try
        {
            accountItem->setText(0, _accountItemText(account));     //  may throw
            accountItem->setIcon(0, account->type()->smallIcon());  //  nothrow
        }
        catch (const tt3::util::Exception & ex)
        {
            accountItem->setText(0, ex.errorMessage());
            accountItem->setIcon(0, errorIcon);
        }
        userItem->addChild(accountItem);
        accountItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(account));
    }
    while (userItem->childCount() > accounts.size())
    {   //  Too many leaf (Account) items
        delete userItem->takeChild(userItem->childCount() - 1);
    }

    //  ...and that each leaf item represents
    //  a proper Account and has proper children
    for (int i = 0; i < accounts.size(); i++)
    {
        QTreeWidgetItem * accountItem = userItem->child(i);
        tt3::ws::Account account = accountItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Account>();
        if (account != accounts[i])
        {   //  Replace tree item data...
            account = accounts[i];
            accountItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(account));
        }
        //  ...adjust its text/icon...
        try
        {
            accountItem->setText(0, _accountItemText(account));     //  may throw
            accountItem->setIcon(0, account->type()->smallIcon());  //  nothrow
        }
        catch (const tt3::util::Exception & ex)
        {
            accountItem->setText(0, ex.errorMessage());
            accountItem->setIcon(0, errorIcon);
        }
    }
}

QString UserManager::_userItemText(tt3::ws::User user) throws(tt3::ws::WorkspaceException)
{
    QString result = user->realName(_credentials);
    if (!user->enabled(_credentials))
    {
        result += " [disabled]";
    }
    return result;
}

QString UserManager::_accountItemText(tt3::ws::Account account) throws(tt3::ws::WorkspaceException)
{
    QString result = account->login(_credentials);
    if (!account->enabled(_credentials))
    {
        result += " [disabled]";
    }
    return result;
}

tt3::ws::User UserManager::_selectedUser()
{
    QTreeWidgetItem * item = _ui->usersTreeWidget->currentItem();
    return (item != nullptr && item->parent() == nullptr) ?
                item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>() :
               nullptr;
}

void UserManager::_setSelectedUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->usersTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * userItem = _ui->usersTreeWidget->topLevelItem(i);
        if (user == userItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>())
        {   //  This one!
            _ui->usersTreeWidget->setCurrentItem(userItem);
            break;
        }
    }
}

tt3::ws::Account UserManager::_selectedAccount()
{
    QTreeWidgetItem * item = _ui->usersTreeWidget->currentItem();
    return (item != nullptr && item->parent() != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Account>() :
               nullptr;
}

//////////
//  Signal handlers
void UserManager::_usersTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    refresh();
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
    CreateUserDialog dlg(this, _workspace, _credentials);
    if (dlg.doModal() == CreateUserDialog::Result::Ok)
    {   //  User created
        refresh();
        _setSelectedUser(dlg.createdUser());
    }
}

void UserManager::_modifyUserPushButtonClicked()
{
    tt3::ws::User user = _selectedUser();
    if (user != nullptr)
    {
        ModifyUserDialog dlg(this, user, _credentials);
        if (dlg.doModal() == ModifyUserDialog::Result::Ok)
        {   //  User modified - its position in the users tree may have changed
            refresh();
            _setSelectedUser(user);
        }
    }
}

void UserManager::_destroyUserPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void UserManager::_createAccountPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void UserManager::_modifyAccountPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void UserManager::_destroyAccountPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

//  End of tt3-gui/UserManager.cpp
