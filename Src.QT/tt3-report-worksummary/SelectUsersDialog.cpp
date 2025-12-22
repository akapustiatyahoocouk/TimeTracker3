//
//  tt3-report-worksummary/SelectUsersDialog.cpp - SelectUsersDialog class implementation
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
#include "tt3-report-worksummary/API.hpp"
#include "ui_SelectUsersDialog.h"
using namespace tt3::report::worksummary;

//////////
//  Construction/destruction
SelectUsersDialog::SelectUsersDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::ReportCredentials & credentials,
        SelectionMode selectionMode,
        const tt3::ws::Users & selectedUsers
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _selectionMode(selectionMode),
        _selectedUsers(selectedUsers),
        //  Controls
        _ui(new Ui::SelectUsersDialog)
{
    _ui->setupUi(this);

    //  Done
    _refresh();
    adjustSize();
    _ui->filterLineEdit->setFocus();
}

SelectUsersDialog::~SelectUsersDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectUsersDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void SelectUsersDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = tt3::gui::RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        try
        {
            //  All users...
            auto users = _workspace->users(_credentials);
            if (QString filter = _ui->filterLineEdit->text().trimmed();
                !filter.isEmpty())  //  TODO can we use the same trick elsewhere ?
            {   //  Perform filtering
                for (auto user : users.values())    //  shallow kind-of-clone
                {
                    if (!user->realName(_credentials).contains(filter, Qt::CaseInsensitive))
                    {   //  Not this one!
                        users.remove(user);
                    }
                }
            }
            _refreshUsersList(_ui->allUsersListWidget, users);
            _refreshSelectionMarks();
            //  ...and chosen users
            _refreshUsersList(_ui->chosenUsersListWidget, _selectedUsers);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & cleanup
            qCritical() << ex;
            _ui->allUsersListWidget->clear();
            _ui->chosenUsersListWidget->clear();
        }
    }
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_selectedUsers.isEmpty());
}

void SelectUsersDialog::_refreshUsersList(
        QListWidget * listWidget,
        const tt3::ws::Users & users
    )
{
    static const QIcon userIcon(":/tt3-report-worksummary/Resources/Images/Objects/UserSmall.png");
    static const QIcon errorIcon(":/tt3-report-worksummary/Resources/Images/Misc/ErrorSmall.png");

    QList<tt3::ws::User> usersList(users.cbegin(), users.cend());
    std::sort(
        usersList.begin(),
        usersList.end(),
        [&](auto a, auto b)
        {
            try
            {
                return tt3::util::NaturalStringOrder::less(
                    a->realName(_credentials),
                    b->realName(_credentials));
            }
            catch (...)
            {   //  OOPS! Revert to stable sort by OID
                return a->oid() < b->oid();
            }
        });
    //  Make sure the list has a proper number...
    while (listWidget->count() < usersList.size())
    {   //  Too few items
        listWidget->addItem(new QListWidgetItem());
    }
    while (listWidget->count() > usersList.size())
    {   //  Too many items
        delete listWidget->takeItem(listWidget->count() - 1);
    }
    //  ...of proper items
    for (int i = 0; i < usersList.size(); i++)
    {
        auto item = listWidget->item(i);
        auto user = usersList[i];
        try
        {
            item->setText(user->realName(_credentials));    //  TODO may throw
            item->setIcon(userIcon);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            item->setText(ex.errorMessage());
            item->setIcon(errorIcon);
        }
        item->setData(Qt::ItemDataRole::UserRole, QVariant::fromValue(user));
    }
}

void SelectUsersDialog::_refreshSelectionMarks()
{
    for (int i = 0; i < _ui->allUsersListWidget->count(); i++)
    {
        auto item = _ui->allUsersListWidget->item(i);
        auto user = item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::User>();
        item->setCheckState(
            _selectedUsers.contains(user) ?
                Qt::CheckState::Checked :
                Qt::CheckState::Unchecked);
    }
}

void SelectUsersDialog::_setSelectedUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->allUsersListWidget->count(); i++)
    {
        auto item = _ui->allUsersListWidget->item(i);
        if (user == item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::User>())
        {
            _ui->allUsersListWidget->setCurrentRow(i);
            break;
        }
    }
}

//////////
//  Signal handlers
void SelectUsersDialog::_filterLineEditTextChanged(QString)
{
    _refresh();
}

void SelectUsersDialog::_allUsersListWidgetItemChanged(QListWidgetItem * item)
{
    if (!_refreshUnderway)
    {
        tt3::ws::User user =
            item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::User>();
        if (item->checkState() == Qt::Checked &&
            !_selectedUsers.contains(user))
        {   //  Item is newly "checked" - add to selection
            if (_selectionMode == SelectionMode::SingleUser)
            {
                _selectedUsers.clear();
            }
            _selectedUsers.insert(user);
        }
        else if (item->checkState() == Qt::Unchecked &&
                 _selectedUsers.contains(user))
        {   //  Item is newly "unckecked" - remove from selection
            _selectedUsers.remove(user);
        }
    }
    _refresh();
}

void SelectUsersDialog::_chosenUsersListWidgetCurrentRowChanged(int)
{
    if (QListWidgetItem * item = _ui->chosenUsersListWidget->currentItem())
    {
        tt3::ws::User user =
            item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::User>();
        _setSelectedUser(user);
    }
    _refresh();
}

void SelectUsersDialog::accept()
{
    done(int(Result::Ok));
}

void SelectUsersDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-report-worksummary/SelectUsersDialog.cpp
