//
//  tt3-tools-backup/BackupTool.hpp - tt3 Backup tool
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

namespace tt3::tools::backup
{
    /// \class BackupTool tt3-tools-backup/API.hpp
    /// \brief The "TT3 Backup" tool.
    class TT3_TOOLS_BACKUP_PUBLIC BackupTool final
        :   public virtual tt3::util::ITool
    {
        DECLARE_SINGLETON(BackupTool)

        //////////
        //  ITool
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        virtual Mnemonic    mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QString     description() const override;
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;
        virtual bool        enabled() const override;
        virtual void        run(QWidget * parent) override;
    };
}

//  End of tt3-tools-backup/Components.hpp
