//
//  tt3-db-api/Capabilities.hpp - login account capabilities
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

namespace tt3::db::api
{
    //  Capabilities of a login account
    enum class Capabilities
    {
        //  Individual capabilty flags
        Administrator = 0x0001,
        ManageUsers = 0x0002,
        ManageActivityTypes = 0x0004,
        ManageBeheficiaries = 0x0008,
        ManageWorkloads = 0x0010,
        ManagePublicActivities = 0x0020,
        ManagePublicTasks = 0x0040,
        ManagePrivateActivities = 0x0080,
        ManagePrivateTasks = 0x0100,
        LogWork = 0x0200,
        LogEvents = 0x0400,
        GenerateReports = 0x0800,
        BackupAndRestore = 0x1000,
        //  Flag combinatons
        None = 0x0000,
        All = 0x1FFF
    };

    TT3_DB_API_PUBLIC inline auto operator & (
            Capabilities a,
            Capabilities b
        ) -> Capabilities
    {
        return Capabilities(int(a) & int(b));
    }

    TT3_DB_API_PUBLIC inline auto operator | (
            Capabilities a,
            Capabilities b
        ) -> Capabilities
    {
        return Capabilities(int(a) | int(b));
    }

    TT3_DB_API_PUBLIC inline auto operator &= (
            Capabilities & a,
            Capabilities b
        ) -> Capabilities &
    {
        a = Capabilities(int(a) & int(b));
        return a;
    }

    TT3_DB_API_PUBLIC inline auto operator |= (
            Capabilities & a,
            Capabilities b
        ) -> Capabilities &
    {
        a = Capabilities(int(a) | int(b));
        return a;
    }

    TT3_DB_API_PUBLIC inline bool operator ! (
            Capabilities a
        )
    {
        return a == Capabilities::None;
    }
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_DB_API_PUBLIC
    QString toString<tt3::db::api::Capabilities>(
            const tt3::db::api::Capabilities & value
        );

    template <> TT3_DB_API_PUBLIC
    auto fromString<tt3::db::api::Capabilities>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::db::api::Capabilities;
}

//  End of tt3-db-api/Capabilities.hpp
