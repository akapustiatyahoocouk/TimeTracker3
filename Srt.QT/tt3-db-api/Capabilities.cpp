//
//  tt3-db-api/Capabilities.cpp - tt3::db::api::Capabilities enum implementation
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
#include "tt3-db-api/API.hpp"
using namespace tt3::db::api;

namespace
{
    struct CapabilityName
    {
        Capabilities    capability;
        QString         name;
    };
    const CapabilityName capabilityNames[] =
    {
        { Capabilities::Administrator, "Administrator" },
        { Capabilities::ManageUsers, "ManageUsers" },
        { Capabilities::ManageActivityTypes, "ManageActivityTypes" },
        { Capabilities::ManageBeheficiaries, "ManageBeheficiaries" },
        { Capabilities::ManageWorkloads, "ManageWorkloads" },
        { Capabilities::ManagePublicActivities, "ManagePublicActivities" },
        { Capabilities::ManagePublicTasks, "ManagePublicTasks" },
        { Capabilities::ManagePrivateActivities, "ManagePrivateActivities" },
        { Capabilities::ManagePrivateTasks, "ManagePrivateTasks" },
        { Capabilities::LogWork, "LogWork" },
        { Capabilities::LogEvents, "LogEvents" },
        { Capabilities::GenerateReports, "GenerateReports" },
        { Capabilities::BackupAndRestore, "BackupAndRestore" }
    };

    const char Separator = '+';
}

template <> TT3_DB_API_PUBLIC QString tt3::util::toString<Capabilities>(const Capabilities & value)
{
    QString result;
    for (size_t i = 0; i < sizeof(capabilityNames) / sizeof(capabilityNames[0]); i++)
    {
        if ((value & capabilityNames[i].capability) != Capabilities::None)
        {
            if (!result.isEmpty())
            {
                result += Separator;
            }
            result += capabilityNames[i].name;
        }
    }
    return result;
}

template <> TT3_DB_API_PUBLIC Capabilities tt3::util::fromString<Capabilities>(const QString & s, int & scan) throws(ParseException)
{
    Capabilities result = Capabilities::None;
    if (scan >= s.length())
    {
        return result;
    }
    int prescan = scan;
    bool separatorConsumed = false;
    for (; ; )
    {
        //  Does a capability name start at s[scan] |
        Capabilities addend = Capabilities::None;
        for (size_t i = 0; i < sizeof(capabilityNames) / sizeof(capabilityNames[0]); i++)
        {
            if (s.mid(prescan).startsWith(capabilityNames[i].name))
            {   //  Yes
                addend = capabilityNames[i].capability;
                prescan += int(capabilityNames[i].name.length());
                break;
            }
        }
        if (addend == Capabilities::None)
        {   //  No more!
            separatorConsumed = false;
            break;
        }
        result |= addend;
        //  More ?
        if (prescan < s.length() && s[prescan] == Separator)
        {
            prescan++;
            separatorConsumed = true;
        }
        else
        {
            separatorConsumed = false;
            break;
        }
    }
    if (separatorConsumed)
    {
        prescan--;
    }
    scan = prescan;
    return result;
}

//  End of tt3-db-api/Capabilities.cpp
