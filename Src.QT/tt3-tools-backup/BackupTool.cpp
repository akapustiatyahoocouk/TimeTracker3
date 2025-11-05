//
//  tt3-tools-backup/BackupTool.cpp - BackupTool class implementation
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
#include "tt3-tools-backup/API.hpp"
using namespace tt3::tools::backup;

//////////
//  Singleton
IMPLEMENT_SINGLETON(BackupTool)
BackupTool::BackupTool() {}
BackupTool::~BackupTool() {}

//////////
//  tt3::uti::ITool
BackupTool::Mnemonic BackupTool::mnemonic() const
{
    return M(Backup);
}

QString BackupTool::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(BackupTool), RID(DisplayName));
}

QString BackupTool::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(BackupTool), RID(Description));
}

QIcon BackupTool::smallIcon() const
{
    static const QIcon icon(":/tt3-tools-backup/Resources/Images/Misc/BackupSmall.png");
    return icon;
}

QIcon BackupTool::largeIcon() const
{
    static const QIcon icon(":/tt3-tools-vackup/Resources/Images/Misc/BackupLarge.png");
    return icon;
}

bool BackupTool::enabled() const
{
    return true;
}

void BackupTool::run(QWidget * parent)
{
    SelectWorkspaceDialog dlg(parent);
    if (dlg.doModal() != SelectWorkspaceDialog::Result::Ok)
    {
        return;
    }
    //  TODO implement
}

//  End of tt3-tools-backup/BackupTool.cpp
