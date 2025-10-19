//
//  tt3-gui/DestroyBeneficiaryDialog.cpp - tt3::gui::DestroyBeneficiaryDialog class implementation
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
using namespace tt3::gui;

//////////
//  Construction/destrution
DestroyBeneficiaryDialog::DestroyBeneficiaryDialog(
        ::QWidget * parent,
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyBeneficiaryLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyBeneficiaryDialog),
                RID(Title)),
            _prompt(beneficiary, credentials)),
        _beneficiary(beneficiary),
        _credentials(credentials)
{
}

//////////
//  Operations
DestroyBeneficiaryDialog::Result DestroyBeneficiaryDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyBeneficiaryDialog::_prompt(
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyBeneficiaryDialog));

    QString result =
        rr.string(
            RID(Prompt),
            beneficiary->displayName(credentials)); //  may throw
    //  TODO extend the message with more info ?
    return result;
}

//////////
//  Signal handlers
void DestroyBeneficiaryDialog::accept()
{
    try
    {
        _beneficiary->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyBeneficiaryDialog.cpp
