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
        Capability  capability;
        QString     name;
    };
#define TT3_WS_CAPABILITY_NAME(C)  { Capability::C, #C }
    const CapabilityName capabilityNames[] =
    {
        TT3_WS_CAPABILITY_NAME(Administrator),
        TT3_WS_CAPABILITY_NAME(ManageUsers),
        TT3_WS_CAPABILITY_NAME(ManageActivityTypes),
        TT3_WS_CAPABILITY_NAME(ManageBeneficiaries),
        TT3_WS_CAPABILITY_NAME(ManageWorkloads),
        TT3_WS_CAPABILITY_NAME(ManagePublicActivities),
        TT3_WS_CAPABILITY_NAME(ManagePublicTasks),
        TT3_WS_CAPABILITY_NAME(ManagePrivateActivities),
        TT3_WS_CAPABILITY_NAME(ManagePrivateTasks),
        TT3_WS_CAPABILITY_NAME(LogWork),
        TT3_WS_CAPABILITY_NAME(LogEvents),
        TT3_WS_CAPABILITY_NAME(GenerateReports),
        TT3_WS_CAPABILITY_NAME(BackupAndRestore)
#undef TT3_WS_CAPABILITY_NAME
    };

    const char Separator = '+';
}

template <> TT3_DB_API_PUBLIC
QString tt3::util::toString<Capabilities>(
        const Capabilities & value
    )
{
    QString result;
    for (size_t i = 0; i < sizeof(capabilityNames) / sizeof(capabilityNames[0]); i++)
    {
        if (value.contains(capabilityNames[i].capability))
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

template <> TT3_DB_API_PUBLIC
auto tt3::util::fromString<Capabilities>(
        const QString & s,
        qsizetype & scan
    ) -> Capabilities
{
    Capabilities result;
    if (scan >= s.length())
    {
        return result;
    }
    qsizetype prescan = scan;
    for (bool separatorConsumed = false; ; )
    {
        //  Does a capability name start at s[scan]?
        Capabilities addend;
        for (size_t i = 0; i < sizeof(capabilityNames) / sizeof(capabilityNames[0]); i++)
        {
            if (s.mid(prescan).startsWith(capabilityNames[i].name))
            {   //  Yes
                addend = capabilityNames[i].capability;
                prescan += int(capabilityNames[i].name.length());
                break;
            }
        }
        if (addend.isEmpty())
        {   //  No more...
            if (separatorConsumed)
            {   //  ...but we've consumed Separator at the end
                //  of the previous iteration
                prescan--;
            }
            break;
        }
        result |= addend;
        separatorConsumed = false;
        //  More ?
        if (prescan < s.length() && s[prescan] == Separator)
        {
            prescan++;
            separatorConsumed = true;
        }
        else
        {
            break;
        }
    }
    scan = prescan;
    return result;
}

//  End of tt3-db-api/Capabilities.cpp
