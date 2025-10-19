//
//  tt3-gui/ModifyProjectDialog.cpp - tt3::gui::ModifyProjectDialog class implementation
//  TODO translate UI via Resources
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
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values (may throw)
    _setSelectedParentProject(_project->parent(_credentials));
    _ui->displayNameLineEdit->setText(_project->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_project->description(_credentials));
    //  TODO beneficiaries
    _ui->completedCheckBox->setChecked(_project->completed(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View project");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewProjectLarge.png"));
        _ui->parentProjectComboBox->setEnabled(false);
        _ui->selectParentProjectPushButton->setEnabled(false);
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
        _ui->beneficiariesValueLabel->setEnabled(false);
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
    _ui->displayNameLineEdit->setFocus();
    adjustSize();
    _refresh();
}

ModifyProjectDialog::~ModifyProjectDialog()
{
    delete _ui;
}

//////////
//  Operations
ModifyProjectDialog::Result ModifyProjectDialog::doModal()
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
    //  Refill the "parent project" combo box
    _ui->parentProjectComboBox->clear();
    _ui->parentProjectComboBox->addItem(
        "- (a root project with no parent)",
        QVariant::fromValue<tt3::ws::Project>(nullptr));
    if (parentProject != nullptr)
    {
        _ui->parentProjectComboBox->addItem(
            parentProject->type()->smallIcon(),
            parentProject->displayName(_credentials),   //  TODO may throw
            QVariant::fromValue(parentProject));
        _ui->parentProjectComboBox->setCurrentIndex(1);
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
        qCritical() << ex.errorMessage();
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
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyProjectDialog::accept()
{
    try
    {
        //  Any of the setters may throw
        if (!_readOnly)
        {
            //  TODO uncomment _project->setParent(_credentials, _selectedParentTask);
            _project->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _project->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _project->setCompleted(
                _credentials,
                _ui->completedCheckBox->isChecked());
            //  TODO Beneficiaries
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        ErrorDialog::show(this, ex);
    }
}

void ModifyProjectDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyProjectDialog.cpp
