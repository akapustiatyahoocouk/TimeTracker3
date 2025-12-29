//
//  tt3-gui/ModifyProjectDialog.cpp - tt3::gui::ModifyProjectDialog class implementation
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
#include "ui_ModifyProjectDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyProjectDialog::ModifyProjectDialog(
        QWidget * parent,
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _project(project),
        _credentials(credentials),
        _validator(project->workspace()->validator()->project()),
        _readOnly(project == nullptr ||
                  !project->canModify(credentials) ||  //  may throw
                  project->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyProjectDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyProjectDialog));

    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    _listWidgetDecorations = ListWidgetDecorations(_ui->beneficiariesListWidget);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->parentProjectLabel->setText(
        rr.string(RID(ParentProjectLabel)));
    _ui->selectParentProjectPushButton->setText(
        rr.string(RID(SelectParentProjectPushButton)));
    _ui->displayNameLabel->setText(
        rr.string(RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        rr.string(RID(DescriptionLabel)));
    _ui->beneficiariesLabel->setText(
        rr.string(RID(BeneficiariesLabel)));
    _ui->selectBeneficiariesPushButton->setText(
        rr.string(RID(SelectBeneficiariesPushButton)));
    _ui->completedCheckBox->setText(
        rr.string(RID(CompletedCheckBox)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values (may throw)
    _setSelectedParentProject(_project->parent(_credentials));
    _ui->displayNameLineEdit->setText(_project->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_project->description(_credentials));
    _setSelectedBeneficiaries(_project->beneficiaries(_credentials));
    _ui->completedCheckBox->setChecked(_project->completed(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        setWindowTitle(rr.string(RID(ViewOnlyTitle)));
        setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewProjectLarge.png"));
        _ui->parentProjectComboBox->setEnabled(false);
        _ui->selectParentProjectPushButton->setEnabled(false);
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
        _ui->beneficiariesListWidget->setEnabled(false);
        _ui->selectBeneficiariesPushButton->setEnabled(false);
        _ui->completedCheckBox->setEnabled(false);
    }
    else if (_project->completed(_credentials))
    {   //  Only an Administrator can "uncomplete"
        //  a Project - the ManageWorkloads capability
        //  is not enough
        _ui->completedCheckBox->setEnabled(
            _project->workspace()->grantsAll( //  may throw
                _credentials,
                tt3::ws::Capability::Administrator));
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyProjectDialog::~ModifyProjectDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyProjectDialog::doModal() -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto ModifyProjectDialog::_selectedParentProject(
    ) -> tt3::ws::Project
{
    return _ui->parentProjectComboBox->currentData().value<tt3::ws::Project>();
}

void ModifyProjectDialog::_setSelectedParentProject(
        tt3::ws::Project parentProject
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyProjectDialog));

    //  Refill the "parent project" combo box
    _ui->parentProjectComboBox->clear();
    _ui->parentProjectComboBox->addItem(
        rr.string(RID(NoParent)),
        QVariant::fromValue<tt3::ws::Project>(nullptr));
    if (parentProject != nullptr)
    {
        try
        {
            _ui->parentProjectComboBox->addItem(
                parentProject->type()->smallIcon(),
                parentProject->displayName(_credentials),   //  may throw
                QVariant::fromValue(parentProject));
            _ui->parentProjectComboBox->setCurrentIndex(1);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & suppress
            qCritical() << ex;
            Q_ASSERT(_ui->parentProjectComboBox->count() == 1);
        }
    }
}

auto ModifyProjectDialog::_selectedBeneficiaries(
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

void ModifyProjectDialog::_setSelectedBeneficiaries(
        tt3::ws::Beneficiaries beneficiaries
    )
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    QList<tt3::ws::Beneficiary> beneficiariesList = beneficiaries.values();
    std::sort(
        beneficiariesList.begin(),
        beneficiariesList.end(),
        [&](auto a, auto b)
        {
            try
            {
                return tt3::util::NaturalStringOrder::less(
                    a->displayName(_credentials),
                    b->displayName(_credentials));  //  may throw
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
    //  ...each represent a proper Workload
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

void ModifyProjectDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void ModifyProjectDialog::_selectParentProjectPushButtonClicked()
{
    try
    {
        SelectProjectParentDialog dlg(
            this, _project, _credentials, _selectedParentProject());
        if (dlg.doModal() == SelectProjectParentDialog::Result::Ok)
        {
            _setSelectedParentProject(dlg.selectedParentProject());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyProjectDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyProjectDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyProjectDialog::_selectBeneficiariesPushButtonClicked()
{
    try
    {
        SelectBeneficiariesDialog dlg(
            this, _project->workspace(), _credentials, _selectedBeneficiaries());
        if (dlg.doModal() == SelectBeneficiariesDialog::Result::Ok)
        {
            _setSelectedBeneficiaries(dlg.selectedBeneficiaries());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyProjectDialog::accept()
{
    try
    {
        //  Any of the setters may throw
        if (!_readOnly)
        {
            _project->setParent(
                _credentials,
                _selectedParentProject());
            _project->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _project->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _project->setCompleted(
                _credentials,
                _ui->completedCheckBox->isChecked());
            _project->setBeneficiaries(
                _credentials,
                _selectedBeneficiaries());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyProjectDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyProjectDialog.cpp
