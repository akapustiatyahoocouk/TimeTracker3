//
//  tt3-ws/WorkspaceAddress.cpp - tt3::ws::WorkspaceAddress class implementation
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
#include "tt3-ws/API.hpp"
using namespace tt3::ws;

//////////
//  Construction/destruction/assignment
WorkspaceAddress::WorkspaceAddress(tt3::db::api::IDatabaseAddress * databaseAddress)
    :   _databaseAddress(nullptr)
{   //  TODO synchronize ?
    _databaseAddress = databaseAddress;
    if (_databaseAddress != nullptr)
    {
        _databaseAddress->addReference();
    }
}

WorkspaceAddress::WorkspaceAddress()
    :   _databaseAddress(nullptr)
{
}

WorkspaceAddress::WorkspaceAddress(const WorkspaceAddress & src)
    :   _databaseAddress(nullptr)
{   //  TODO synchronize ?
    _databaseAddress = src._databaseAddress;
    if (_databaseAddress != nullptr)
    {
        _databaseAddress->addReference();
    }
}

WorkspaceAddress::WorkspaceAddress(WorkspaceAddress && src)
    :   _databaseAddress(nullptr)
{   //  TODO synchronize ?
    _databaseAddress = src._databaseAddress;
    src._databaseAddress = nullptr;
}

WorkspaceAddress::~WorkspaceAddress()
{   //  TODO synchronize ?
    if (_databaseAddress != nullptr)
    {
        _databaseAddress->removeReference();
    }
}

WorkspaceAddress & WorkspaceAddress::operator = (const WorkspaceAddress & src)
{
    if (this != &src)
    {   //  TODO synchronize ?
        if (_databaseAddress != nullptr)
        {
            _databaseAddress->removeReference();
        }
        _databaseAddress = src._databaseAddress;
        if (_databaseAddress != nullptr)
        {
            _databaseAddress->addReference();
        }
    }
    return *this;
}

WorkspaceAddress & WorkspaceAddress::operator = (WorkspaceAddress && src)
{   //  TODO synchronize ?
    std::swap(_databaseAddress, src._databaseAddress);
    return *this;
}

//////////
//  Operators
bool WorkspaceAddress::operator == (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress == op2._databaseAddress;
}

bool WorkspaceAddress::operator != (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress != op2._databaseAddress;
}

bool WorkspaceAddress::operator <  (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress < op2._databaseAddress;
}

bool WorkspaceAddress::operator <= (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress == op2._databaseAddress;
}

bool WorkspaceAddress::operator >  (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress > op2._databaseAddress;
}

bool WorkspaceAddress::operator >= (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress >= op2._databaseAddress;
}

//////////
//  Operations (general)
bool WorkspaceAddress::isValid() const
{   //  TODO synchronize ?
    return _databaseAddress != nullptr;
}

WorkspaceType * WorkspaceAddress::workspaceType() const
{   //  TODO synchronize ?
    return (_databaseAddress == nullptr) ?
                nullptr :
                WorkspaceTypeRegistry::_findWorkspaceType(_databaseAddress->databaseType());
}

QString WorkspaceAddress::displayForm() const
{   //  TODO synchronize ?
    return (_databaseAddress == nullptr) ?
                "" :
                _databaseAddress->displayForm();
}

QString WorkspaceAddress::externalForm() const
{   //  TODO synchronize ?
    return (_databaseAddress == nullptr) ?
                "" :
                _databaseAddress->externalForm();
}

//////////
//  Formatting/parsing
template <> TT3_WS_PUBLIC QString tt3::util::toString<WorkspaceAddress>(const WorkspaceAddress & value)
{
    if (!value.isValid())
    {
        return "-";
    }
    QString m = value.workspaceType()->mnemonic();;
    QString a = value.externalForm();
    //  We need a mnemonic/address separator not used by either part
    QSet<QChar> usedCharacters;
    usedCharacters += QSet<QChar>(m.cbegin(), m.cend());
    usedCharacters += QSet<QChar>(a.cbegin(), a.cend());

    QChar chunkSeparator = '\t';
    if (usedCharacters.contains(chunkSeparator))
    {   //  Default won't do
        for (int code = 256; ; code++)
        {
            chunkSeparator = QChar(code);
            if (!usedCharacters.contains(chunkSeparator))
            {
                break;
            }
        }
    }
    usedCharacters.insert(chunkSeparator);

    //  Build the result
    return chunkSeparator + m + chunkSeparator + a + chunkSeparator;
}

template <> TT3_WS_PUBLIC QString tt3::util::toString<WorkspaceAddressesList>(const tt3::ws::WorkspaceAddressesList & value)
{
    QStringList addresses;
    for (WorkspaceAddress a : value)
    {
        if (a.isValid())
        {
            addresses.append(toString(a));
        }
    }
    QSet<QChar> usedCharacters;
    for (QString a : addresses)
    {
        usedCharacters += QSet<QChar>(a.cbegin(), a.cend());
    }

    //  We need a separator for the items list
    QChar itemSeparator = '\n';
    if (usedCharacters.contains(itemSeparator))
    {   //  Default won't do
        for (int code = 256; ; code++)
        {
            itemSeparator = QChar(code);
            if (!usedCharacters.contains(itemSeparator))
            {
                break;
            }
        }
    }
    usedCharacters.insert(itemSeparator);

    //  Assemble the result
    QString result;
    result += itemSeparator;
    for (int i = 0; i < addresses.size(); i++)
    {
        result += addresses[i];
        if (i + 1 < addresses.size())
        {
            result += itemSeparator;
        }
    }
    result += itemSeparator;
    result += itemSeparator;    //  double item separator == end of list
    return result;
}

template <> TT3_WS_PUBLIC tt3::ws::WorkspaceAddress tt3::util::fromString<tt3::ws::WorkspaceAddress>(const QString & s, int & scan) throws(ParseException)
{
    //  Handle special cases
    if (scan < s.length() && s[scan] == '?')
    {
        scan++;
        return WorkspaceAddress();  //  invalid address
    }

    //  General case
    if (scan + 3 >= s.length())
    {
        throw tt3::util::ParseException(s, scan);
    }

    QChar chunkSeparator = s[scan];
    //  Where is the chunk separator 2 ?
    int prescan2;
    for (prescan2 = scan + 1; prescan2 < s.length(); prescan2++)
    {
        if (s[prescan2] == chunkSeparator)
        {   //  Here!
            break;
        }
    }
    if (prescan2 == s.length())
    {   //  OOPS Not found!
        throw tt3::util::ParseException(s, scan);
    }
    QString workspaceTypeMnenonic = s.mid(scan + 1, prescan2 - scan - 1);
    //  Where is the chunk separator 3 ?
    int prescan3;
    for (prescan3 = prescan2 + 1; prescan3 < s.length(); prescan3++)
    {
        if (s[prescan3] == chunkSeparator)
        {   //  Here!
            break;
        }
    }
    if (prescan3 == s.length())
    {   //  OOPS Not found!
        throw tt3::util::ParseException(s, scan);
    }
    QString addressExternalForm = s.mid(prescan2 + 1, prescan3 - prescan2);
    tt3::ws::WorkspaceType * workspaceType =
        tt3::ws::WorkspaceTypeRegistry::findWorkspaceType(workspaceTypeMnenonic);
    if (workspaceType == nullptr)
    {   //  OOPS! Not supported!
        return WorkspaceAddress();  //  invalid address
    }
    try
    {
        tt3::ws::WorkspaceAddress workspaceAddress =
            workspaceType->parseWorkspaceAddress(addressExternalForm);
        scan = prescan3 + 1;
        return workspaceAddress;
    }
    catch (...)
    {   //  OOPS!@ Can't!
        throw tt3::util::ParseException(s, scan);
    }
}

template <> TT3_WS_PUBLIC tt3::ws::WorkspaceAddressesList tt3::util::fromString<tt3::ws::WorkspaceAddressesList>(const QString & s, int & scan) throws(ParseException)
{
}

//  End of tt3-ws/WorkspaceAddress.cpp
