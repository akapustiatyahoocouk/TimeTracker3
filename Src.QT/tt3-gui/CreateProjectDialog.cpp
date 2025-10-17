//
//  tt3-gui/CreateProjectDialog.cpp - tt3::gui::CreateProjectDialog class implementation
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
#include "ui_CreateProjectDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateProjectDialog::CreateProjectDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        tt3::ws::Project initialParentProject
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->project()),
        //  Controls
        _ui(new Ui::CreateProjectDialog)
{
    Q_ASSERT(_credentials.isValid());
    Q_ASSERT(initialParentProject == nullptr ||
             initialParentProject->workspace() == _workspace);

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values (may throw)
    _setSelectedParentProject(initialParentProject);

    //  Done
    _ui->displayNameLineEdit->setFocus();
    adjustSize();
    _refresh();
}

CreateProjectDialog::~CreateProjectDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateProjectDialog::Result CreateProjectDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto CreateProjectDialog::_selectedParentProject(
    ) -> tt3::ws::Project
{
    return _ui->parentProjectComboBox->currentData().value<tt3::ws::Project>();
}

void CreateProjectDialog::_setSelectedParentProject(
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
            parentProject->displayName(_credentials),
            QVariant::fromValue(parentProject));
        _ui->parentProjectComboBox->setCurrentIndex(1);
    }
}

void CreateProjectDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void CreateProjectDialog::_selectParentProjectPushButtonClicked()
{
    try
    {
        SelectProjectParentDialog dlg(
            this, _workspace, _credentials, _selectedParentProject());
        if (dlg.doModal() == SelectProjectParentDialog::Result::Ok)
        {
            _setSelectedParentProject(dlg.selectedParentProject());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void CreateProjectDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreateProjectDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void CreateProjectDialog::_selectBeneficiariesPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void CreateProjectDialog::accept()
{
    try
    {
        if (tt3::ws::Project parentProject = _selectedParentProject())
        {   //  Create child Project
            _createdProject =
                parentProject->createChild(
                    _credentials,
                    _ui->displayNameLineEdit->text(),
                    _ui->descriptionPlainTextEdit->toPlainText(),
                    tt3::ws::Beneficiaries(),   //  TODO for now
                    _ui->completedCheckBox->isChecked());
        }
        else
        {   //  Create root Project
            _createdProject =
                _workspace->createProject(
                    _credentials,
                    _ui->displayNameLineEdit->text(),
                    _ui->descriptionPlainTextEdit->toPlainText(),
                    tt3::ws::Beneficiaries(),   //  TODO for now
                    _ui->completedCheckBox->isChecked());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void CreateProjectDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateProjectDialog.cpp
