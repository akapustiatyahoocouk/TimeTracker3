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

    int xdigit(const QChar & c)
    {
        if (c >= '0' && c <= '9')
        {
            return c.unicode() - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            return c.unicode() - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            return c.unicode() - 'A' + 10;
        }
        else
        {
            return -1;
        }
    }

    QString unescape(const QString & s) throws (ParseExce[tion)
    {
        QString result;
        for (int i = 0; i < s.length(); )
        {
            if (s[i] != '\\')
            {   //  A literal character
                result += s[i];
                i++;
                continue;
            }
            //  We have ab escape sequence. Skip '\'
            i++;
            if (i >= s.length())
            {   //  OOP! Missing
                throw tt3::util::ParseException(s, 0);
            }
            //  Special character ?
            if (s[i] == 'a')
            {   //  \a
                result += '\a';
                i++;
            }
            else if (s[i] == 'b')
            {   //  \b
                result += '\b';
                i++;
            }
            else if (s[i] == 'f')
            {   //  \f
                result += '\f';
                i++;
            }
            else if (s[i] == 'n')
            {   //  \n
                result += '\n';
                i++;
            }
            else if (s[i] == 'r')
            {   //  \r
                result += '\r';
                i++;
            }
            else if (s[i] == 't')
            {   //  \t
                result += '\t';
                i++;
            }
            else if (s[i] == 'v')
            {   //  \a
                result += '\v';
                i++;
            }
            //  Numeric escape ?
            else if (s[i] == 'x')
            {   //  \xXX
                i++;
                if (i + 1 < s.length() &&
                    xdigit(s[i]) != -1 && xdigit(s[i + 1]) != -1)
                {
                    result += QChar(xdigit(s[i]) * 16 +
                                    xdigit(s[i + 1]));
                    i += 2;
                }
                else
                {   //  OOPS! Invalid hex escape!
                    throw tt3::util::ParseException(s, 0);
                }
            }
            else if (s[i] == 'u')
            {   //  \uXXXX
                i++;
                if (i + 3 < s.length() &&
                    xdigit(s[i]) != -1 && xdigit(s[i + 1]) != -1 &&
                    xdigit(s[i + 2]) != -1 && xdigit(s[i + 3]) != -1)
                {
                    result += QChar(xdigit(s[i]) * 4096 +
                                    xdigit(s[i + 1]) * 256 +
                                    xdigit(s[i + 2]) * 16 +
                                    xdigit(s[i + 3]));
                    i += 4;
                }
                else
                {   //  OOPS! Invalid hex escape!
                    throw tt3::util::ParseException(s, 0);
                }
            }
            //  Literal escape
            else
            {
                result += s[i++];
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
    WorkspaceType * workspaceType =
        WorkspaceTypeRegistry::findWorkspaceType(unescape(chunks[0]));
    if (workspaceType == nullptr)
    {   //  OOPS!
        throw tt3::util::ParseException(s, scan);
    }
    //  Parse address
    try
    {
        WorkspaceAddress address =
            workspaceType->parseWorkspaceAddress(unescape(chunks[1]));
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
