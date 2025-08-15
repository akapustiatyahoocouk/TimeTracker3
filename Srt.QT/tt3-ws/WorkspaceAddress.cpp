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
namespace
{
    QString escape(const QString & s)
    {
        char escape[8];

        QString result;
        for (QChar c : s)
        {
            switch (c.unicode())
            {
                //  special characters
            case '\a':
                result += "\\a";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            case '\v':
                result += "\\v";
                break;
                //  literal escapes
            case '<':   case '>':
            case '[':   case ']':
            case ',':   case '\\':
                result += '\\';
                result += c;
                break;
                //  default case
            default:
                if (c.unicode() > 255)
                {   //  0uXXXX
                    sprintf(escape, "\\u%04X", c.unicode());
                    result += escape;
                }
                else if (c < '\x20' || c >= '\x7F')
                {   //  \xXX
                    sprintf(escape, "\\x%02X", c.unicode());
                    result += escape;
                }
                else
                {
                    result += c;
                }
                break;
            }
        }
        return result;
    }
}

template <> TT3_WS_PUBLIC QString tt3::util::toString<WorkspaceAddress>(const WorkspaceAddress & value)
{
    if (value.isValid())
    {
        return '<' +
               escape(value.workspaceType()->mnemonic()) +
               ',' +
               escape(value.externalForm()) +
               '>';
    }
    return "<>";
}

template <> TT3_WS_PUBLIC QString tt3::util::toString<WorkspaceAddressesList>(const tt3::ws::WorkspaceAddressesList & value)
{
    QString result;
    result += '[';
    for (int i = 0; i < value.size(); i++)
    {
        result += toString(value[i]);
        if (i + 1 < value.size())
        {
            result += ',';
        }
    }
    result += ']';
    return result;
}

template <> TT3_WS_PUBLIC tt3::ws::WorkspaceAddress tt3::util::fromString<tt3::ws::WorkspaceAddress>(const QString & s, int & scan) throws(ParseException)
{
    //  Skip '<'
    if (scan >= s.length() || s[scan] != '<')
    {
        throw tt3::util::ParseException(s, scan);
    }
    int prescan;
    //  Find '>'
    for (prescan = scan + 1; prescan < s.length() && s[prescan] != '>'; prescan++)
    {   //  All work done in the loop header
    }
    if (prescan == s.length())
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Break into <mnemonic>,<address> parts
    QStringList chunks = s.mid(scan + 1, prescan - scan - 1).split(',');
    if (chunks.size() != 2)
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Resolve mnemonic
    WorkspaceType * workspaceType = WorkspaceTypeRegistry::findWorkspaceType(chunks[0]);
    if (workspaceType == nullptr)
    {   //  OOPS!
        throw tt3::util::ParseException(s, scan);
    }
    //  Parse address
    try
    {
        WorkspaceAddress address = workspaceType->parseWorkspaceAddress(chunks[1]);
        scan = prescan + 1;
        return address;
    }
    catch (...)
    {   //  OOPS! Must translate
        throw tt3::util::ParseException(s, scan);
    }
}

template <> TT3_WS_PUBLIC tt3::ws::WorkspaceAddressesList tt3::util::fromString<tt3::ws::WorkspaceAddressesList>(const QString & s, int & scan) throws(ParseException)
{
    //  Skip '['
    if (scan >= s.length() || s[scan] != '[')
    {
        throw tt3::util::ParseException(s, scan);
    }
    int prescan = scan + 1;
    //  Parse list items
    WorkspaceAddressesList result;
    if (prescan < s.length() && s[prescan] != ']')
    {   //  At least 1 item exists
        for (; ; )
        {
            result.append(fromString<WorkspaceAddress>(s, prescan));    //  may throw
            //  More ?
            if (prescan < s.length() && s[prescan] == ',')
            {   //  yes
                prescan++;
            }
            else
            {   //  no
                break;
            }
        }
    }
    //  Skip ']'
    if (prescan >= s.length() || s[prescan] != ']')
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Done
    scan = prescan + 1;
    return result;
}

//  End of tt3-ws/WorkspaceAddress.cpp
