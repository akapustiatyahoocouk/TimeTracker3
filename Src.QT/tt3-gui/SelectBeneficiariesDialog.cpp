//
//  tt3-gui/SelectWorkloadsDialog.cpp - tt3::gui::SelectWorkloadsDialog class implementation
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
#include "ui_SelectBeneficiariesDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectBeneficiariesDialog::SelectBeneficiariesDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const tt3::ws::Beneficiaries & beneficiaries
    ) : QDialog(parent),
        _workspace(workspace),
        _credentials(credentials),
        _ui(new Ui::SelectBeneficiariesDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectBeneficiariesDialog));

    Q_ASSERT(_workspace != nullptr);
    _ui->setupUi(this);
    _treeWidgetDecorations = TreeWidgetDecorations(_ui->beneficiariesTreeWidget);
    _listWidgetDecorations = ListWidgetDecorations(_ui->beneficiariesListWidget);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->selectedBeneficiariesLabel->setText(
        rr.string(RID(SelectedBeneficiariesLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _setSelectedBeneficiaries(beneficiaries);

    //  Done
    _refresh();
    adjustSize();
}

SelectBeneficiariesDialog::~SelectBeneficiariesDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectBeneficiariesDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

auto SelectBeneficiariesDialog::selectedBeneficiaries(
    ) -> tt3::ws::Beneficiaries
{
    return _selectedBeneficiaries();
}

//////////
//  Implementation helpers
auto SelectBeneficiariesDialog::_currentBeneficiary(
    ) -> tt3::ws::Beneficiary
{
    if (QTreeWidgetItem * item = _ui->beneficiariesTreeWidget->currentItem())
    {
        return item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>();
    }
    return nullptr;
}

void SelectBeneficiariesDialog::_setCurrentBeneficiary(
        tt3::ws::Beneficiary beneficiary
    )
{
    for (int i = 0; i < _ui->beneficiariesTreeWidget->topLevelItemCount(); i++)
    {
        auto item = _ui->beneficiariesTreeWidget->topLevelItem(i);
        if (beneficiary == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>())
        {
            _ui->beneficiariesTreeWidget->setCurrentItem(item);
            // break;
        }
    }
}

auto SelectBeneficiariesDialog::_selectedBeneficiaries(
    ) -> tt3::ws::Beneficiaries
{
    tt3::ws::Beneficiaries result;
    for (int i = 0; i < _ui->beneficiariesListWidget->count(); i++)
    {
        result.insert(
            _ui->beneficiariesListWidget->item(i)->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>());
    }
    return result;
}

void SelectBeneficiariesDialog::_setSelectedBeneficiaries(
        const tt3::ws::Beneficiaries & beneficiaries
    )
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    QList<tt3::ws::Beneficiary>beneficiariesList = beneficiaries.values();
    std::sort(
        beneficiariesList.begin(),
        beneficiariesList.end(),
        [&](auto a, auto b)
        {
            try
            {
                return a->displayName(_credentials) < b->displayName(_credentials); //  may throw
            }
            catch (tt3::util::Exception & ex)
            {   //  OOPS! Report & recover with a stable sort order
                qCritical() << ex;
                return a->oid() < b->oid();
            }
        });
    //  Make sure a proper number of list widget items...
    while (_ui->beneficiariesListWidget->count() < beneficiariesList.size())
    {   //  Too few items in the list widget
        _ui->beneficiariesListWidget->addItem("?");
    }
    while (_ui->beneficiariesListWidget->count() > beneficiariesList.size())
    {   //  Too many items in the list widget
        delete _ui->beneficiariesListWidget->takeItem(
            _ui->beneficiariesListWidget->count() - 1);
    }
    //  ...each represent a proper Beneficiary
    for (int i = 0; i < beneficiariesList.count(); i++)
    {
        QListWidgetItem * item = _ui->beneficiariesListWidget->item(i);
        tt3::ws::Beneficiary beneficiary = beneficiariesList[i];
        try
        {
            item->setText(beneficiary->displayName(_credentials));  //  may throw
            item->setIcon(beneficiary->type()->smallIcon());
            item->setForeground(_listWidgetDecorations.itemForeground);
        }
        catch (tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex;
            item->setText(ex.errorMessage());
            item->setIcon(errorIcon);
            item->setForeground(_listWidgetDecorations.errorItemForeground);
        }
        item->setData(Qt::ItemDataRole::UserRole, QVariant::fromValue(beneficiary));
    }
}

void SelectBeneficiariesDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        BeneficiaryManager::_WorkspaceModel workspaceModel =
            BeneficiaryManager::_createWorkspaceModel(
                _workspace, _credentials, _treeWidgetDecorations);
        QString filter = _ui->filterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            BeneficiaryManager::_filterItems(
                workspaceModel, filter, _treeWidgetDecorations);
        }
        BeneficiaryManager::_refreshWorkspaceTree(
            _ui->beneficiariesTreeWidget, workspaceModel);
        _refreshBeneficiaryCheckStates();
    }
}

void SelectBeneficiariesDialog::_refreshBeneficiaryCheckStates()
{
    tt3::ws::Beneficiaries selectedBeneficiaries = _selectedBeneficiaries();
    for (int i = 0; i < _ui->beneficiariesTreeWidget->topLevelItemCount(); i++)
    {
        _refreshBeneficiaryCheckStates(_ui->beneficiariesTreeWidget->topLevelItem(i), selectedBeneficiaries);
    }
}

void SelectBeneficiariesDialog::_refreshBeneficiaryCheckStates(
        QTreeWidgetItem * beneficiaryItem,
        const tt3::ws::Beneficiaries & selectedBeneficiaries
    )
{
    tt3::ws::Beneficiary beneficiary =
        beneficiaryItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>();
    beneficiaryItem->setFlags(beneficiaryItem->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    beneficiaryItem->setCheckState(
        0,
        selectedBeneficiaries.contains(beneficiary) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
}

//////////
//  Event handlers
void SelectBeneficiariesDialog::_filterLineEditTextChanged(QString)
{
    _refresh();
}

void SelectBeneficiariesDialog::_beneficiariesTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::Beneficiaries selectedBeneficiaries = _selectedBeneficiaries();
        tt3::ws::Beneficiary beneficiary =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>();
        if (item->checkState(0) == Qt::Checked &&
            !selectedBeneficiaries.contains(beneficiary))
        {   //  Item is newly "checked" - add to selection
            selectedBeneficiaries.insert(beneficiary);
            _setSelectedBeneficiaries(selectedBeneficiaries);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 selectedBeneficiaries.contains(beneficiary))
        {   //  Item is newly "unckecked" - remove from selection
            selectedBeneficiaries.remove(beneficiary);
            _setSelectedBeneficiaries(selectedBeneficiaries);
        }
    }
    _refresh();
}

void SelectBeneficiariesDialog::_beneficiariesListWidgetCurrentRowChanged(int)
{
    if (QListWidgetItem * item = _ui->beneficiariesListWidget->currentItem())
    {
        tt3::ws::Beneficiary b =
            item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>();
        _setCurrentBeneficiary(b);
    }
    _refresh();
}

void SelectBeneficiariesDialog::accept()
{
    done(int(Result::Ok));
}

void SelectBeneficiariesDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectWorkloadsDialog.cpp
