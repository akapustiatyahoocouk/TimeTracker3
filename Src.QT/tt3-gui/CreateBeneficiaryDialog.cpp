//
//  tt3-gui/CreateBeneficiaryDialog.cpp - tt3::gui::CreateBeneficiaryDialog class implementation
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
#include "ui_CreateBeneficiaryDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateBeneficiaryDialog::CreateBeneficiaryDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->beneficiary()),
        //  Controls
        _ui(new Ui::CreateBeneficiaryDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreateBeneficiaryDialog));

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));
    _listWidgetDecorations = ListWidgetDecorations(_ui->workloadsListWidget);

    //  Set up initial control values
    _ui->displayNameLabel->setText(
        rr.string(RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        rr.string(RID(DescriptionLabel)));
    _ui->workloadsLabel->setText(
        rr.string(RID(WorkloadsLabel)));
    _ui->selectWorkloadsPushButton->setText(
        rr.string(RID(SelectWorkloadsPushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Done TODO use this sequence & explicit setFocus() in LL dialogs!
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

CreateBeneficiaryDialog::~CreateBeneficiaryDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateBeneficiaryDialog::Result CreateBeneficiaryDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto CreateBeneficiaryDialog::_selectedWorkloads(
    ) -> tt3::ws::Workloads
{
    tt3::ws::Workloads result;
    for (int i = 0; i < _ui->workloadsListWidget->count(); i++)
    {
        result.insert(
            _ui->workloadsListWidget->item(i)->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Workload>());
    }
    return result;
}

void CreateBeneficiaryDialog::_setSelectedWorkloads(
        const tt3::ws::Workloads & workloads
    )
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    QList<tt3::ws::Workload> workloadsList = workloads.values();
    std::sort(
        workloadsList.begin(),
        workloadsList.end(),
        [&](auto a, auto b)
        {
            try
            {
                return a->displayName(_credentials) < b->displayName(_credentials); //  may throw
            }
            catch (tt3::util::Exception & ex)
            {   //  OOPS! Report & recover with a stable sort order
                qCritical() << ex.errorMessage();
                return a->oid() < b->oid();
            }
        });
    //  Make sure a proper number of list widget items...
    while (_ui->workloadsListWidget->count() < workloadsList.size())
    {   //  Too few items in the list widget
        _ui->workloadsListWidget->addItem("?");
    }
    while (_ui->workloadsListWidget->count() > workloadsList.size())
    {   //  Too many items in the list widget
        delete _ui->workloadsListWidget->takeItem(
            _ui->workloadsListWidget->count() - 1);
    }
    //  ...each represent a proper Workload
    for (int i = 0; i < workloadsList.count(); i++)
    {
        QListWidgetItem * item = _ui->workloadsListWidget->item(i);
        tt3::ws::Workload workload = workloadsList[i];
        try
        {
            QString text = workload->displayName(_credentials); //  may throw
            item->setIcon(workload->type()->smallIcon());
            if (auto project =
                std::dynamic_pointer_cast<tt3::ws::ProjectImpl>(workload))
            {
                if (project->completed(_credentials))  //  may throw
                {   //  Completed
                    text += " [completed]";
                    item->setForeground(_listWidgetDecorations.disabledItemForeground);
                }
                else
                {   //  Not completed
                    item->setForeground(_listWidgetDecorations.itemForeground);
                }
            }
            else
            {   //  Not a task
                item->setForeground(_listWidgetDecorations.itemForeground);
            }
            item->setText(text);
        }
        catch (tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex.errorMessage();
            item->setText(ex.errorMessage());
            item->setIcon(errorIcon);
            item->setForeground(_listWidgetDecorations.errorItemForeground);
        }
        item->setData(Qt::ItemDataRole::UserRole, QVariant::fromValue(workload));
    }
}

void CreateBeneficiaryDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void CreateBeneficiaryDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreateBeneficiaryDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void CreateBeneficiaryDialog::_selectWorkloadsPushButtonClicked()
{
    try
    {
        SelectWorkloadsDialog dlg(this, _workspace, _credentials, _selectedWorkloads());
        if (dlg.doModal() == SelectWorkloadsDialog::Result::Ok)
        {
            _setSelectedWorkloads(dlg.selectedWorkloads());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        ErrorDialog::show(this, ex);
    }
}

void CreateBeneficiaryDialog::accept()
{
    try
    {
        _createdBeneficiary =
            _workspace->createBeneficiary(
                _credentials,
                _ui->displayNameLineEdit->text(),
                _ui->descriptionPlainTextEdit->toPlainText(),
                _selectedWorkloads());
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        ErrorDialog::show(this, ex);
    }
}

void CreateBeneficiaryDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateBeneficiaryDialog.cpp
