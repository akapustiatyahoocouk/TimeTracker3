//
//  tt3-tools-restore/RestoreTool.cpp - RestoreTool class implementation
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
#include "tt3-tools-restore/API.hpp"
using namespace tt3::tools::restore;

//////////
//  Constants
const QString RestoreTool::PreferredFilenameExtension = ".tt3-backup";

//////////
//  Singleton
IMPLEMENT_SINGLETON(RestoreTool)
RestoreTool::RestoreTool() {}
RestoreTool::~RestoreTool() {}

//////////
//  tt3::uti::ITool
RestoreTool::Mnemonic RestoreTool::mnemonic() const
{
    return M(Restore);
}

QString RestoreTool::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(RestoreTool), RID(DisplayName));
}

QString RestoreTool::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(RestoreTool), RID(Description));
}

QIcon RestoreTool::smallIcon() const
{
    static const QIcon icon(":/tt3-tools-restore/Resources/Images/Misc/RestoreSmall.png");
    return icon;
}

QIcon RestoreTool::largeIcon() const
{
    static const QIcon icon(":/tt3-tools-restore/Resources/Images/Misc/RestoreLarge.png");
    return icon;
}

bool RestoreTool::enabled() const
{
    return true;
}

void RestoreTool::run(QWidget * parent)
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);

    ConfigureRestoreDialog dlg(parent);
    if (dlg.doModal() != ConfigureRestoreDialog::Result::Ok)
    {
        return;
    }
}

//  End of tt3-tools-restore/RestoreTool.cpp

