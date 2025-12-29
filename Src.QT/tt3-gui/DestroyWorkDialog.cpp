//
//  tt3-gui/DestroyWorkDialog.cpp - tt3::gui::DestroyWorkDialog class implementation
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
DestroyWorkDialog::DestroyWorkDialog(
            ::QWidget * parent,
            tt3::ws::Work work,
            const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyWorkLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyWorkDialog),
                RID(Title)),
            _prompt(work, credentials)),
        _work(work),
        _credentials(credentials)
{
}

//////////
//  Operations
auto DestroyWorkDialog::doModal() -> Result
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyWorkDialog::_prompt(
        tt3::ws::Work work,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyWorkDialog));

    qint64 secs = qMax(0, work->startedAt(credentials).secsTo(work->finishedAt(credentials)));
    char duration[32];
    sprintf(duration, " [%d:%02d:%02d]",
            int(secs / (60 * 60)),
            int((secs / 60) % 60),
            int(secs % 60));

    QString result =
        rr.string(
            RID(Prompt),
            QLocale().toString(work->startedAt(credentials).toLocalTime(), QLocale::ShortFormat),
            QLocale().toString(work->finishedAt(credentials).toLocalTime(), QLocale::ShortFormat),
            duration,
            work->account(credentials)->login(credentials),
            work->activity(credentials)->type()->displayName().toLower(),
            work->activity(credentials)->displayName(credentials));
    return result;
}

//////////
//  Signal handlers
void DestroyWorkDialog::accept()
{
    try
    {   //  Destroy!
        _work->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyWorkDialog.cpp
