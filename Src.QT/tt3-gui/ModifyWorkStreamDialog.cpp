//
//  tt3-gui/ModifyWorkStreamDialog.cpp - tt3::gui::ModifyWorkStreamDialog class implementation
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
#include "ui_ModifyWorkStreamDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyWorkStreamDialog::ModifyWorkStreamDialog(
        QWidget * parent,
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workStream(workStream),
        _credentials(credentials),
        _validator(workStream->workspace()->validator()->workStream()),
        _readOnly(workStream == nullptr ||
                  !workStream->canModify(credentials) ||  //  may throw
                  workStream->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyWorkStreamDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyWorkStreamDialog));

    Q_ASSERT(_workStream != nullptr);
    Q_ASSERT(_credentials.isValid());
    Q_ASSERT(_validator != nullptr);

    _ui->setupUi(this);
    _listWidgetDecorations = ListWidgetDecorations(_ui->beneficiariesListWidget);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->displayNameLabel->setText(
        rr.string(RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        rr.string(RID(DescriptionLabel)));
    _ui->beneficiariesLabel->setText(
        rr.string(RID(BeneficiariesLabel)));
    _ui->selectBeneficiariesPushButton->setText(
        rr.string(RID(SelectBeneficiariesPushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values (may throw)
    _ui->displayNameLineEdit->setText(_workStream->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_workStream->description(_credentials));
    _setSelectedBeneficiaries(_workStream->beneficiaries(_credentials));

    //  Adjust controls
    if (_readOnly)
    {   //  Adjust for "view only" mode
        setWindowTitle(rr.string(RID(ViewOnlyTitle)));
        setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewWorkStreamLarge.png"));
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyWorkStreamDialog::~ModifyWorkStreamDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyWorkStreamDialog::doModal() -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto ModifyWorkStreamDialog::_selectedBeneficiaries(
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

void ModifyWorkStreamDialog::_setSelectedBeneficiaries(
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
                    b->displayName(_credentials)); //  may throw
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

void ModifyWorkStreamDialog::_refresh()
{
    _ui->selectBeneficiariesPushButton->setEnabled(!_readOnly);
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void ModifyWorkStreamDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyWorkStreamDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyWorkStreamDialog::_selectBeneficiariesPushButtonClicked()
{
    try
    {
        SelectBeneficiariesDialog dlg(
            this, _workStream->workspace(), _credentials, _selectedBeneficiaries());
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

void ModifyWorkStreamDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            _workStream->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _workStream->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _workStream->setBeneficiaries(
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

void ModifyWorkStreamDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyWorkStreamDialog.cpp
