//
//  Xml.cpp - Implementation of the carl::Xml class
//
//  Copyright (C) 2010-2021, Cybernetic Intelligence GmbH
//
//  $Date: 2021-02-27 00:29:32 +0000 (Sat, 27 Feb 2021) $
//  $Revision: 861 $
//  $Author: akapusti $
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//////////
#include "CARL.hpp"
using namespace carl;

//////////
//  Operations
bool Xml::isValidName(const String & name)
{
    if (name.length() == 0)
    {
        return false;
    }
    if (!isValidNameStart(name[0]))
    {
        return false;
    }
    for (UIntSize i = 1; i < name.length(); i++)
    {
        if (!isValidNameChar(name[i]))
        {
            return false;
        }
    }
    return true;
}

bool Xml::isValidNameStart(Char c)
{
    return c.isAlpha() || c == '_' || c == ':';
}

bool Xml::isValidNameChar(Char c)
{
    return c.isAlNum() || c == '.' || c == '-' || c == '_' || c == ':'; //  TODO CombiningChar + Extender
}

String Xml::encodeXmlEntities(const String & s)
{
    static _EntityRegistry * entityRegistry = _EntityRegistry::getInstance();   //  idempotent
    static NumberFormat * numberFormat = NumberFormat::getInstance(Locale::Invariant);  //  idempotent

    String result, entity;
    for (UIntSize i = 0; i < s.length(); i++)
    {
        Char c = s[i];
        if (entityRegistry->findEntityForChar(c, entity))
        {   //  "c" can be represented with an entity
            result += entity;
        }
        else if (c < 32)
        {   //  "c" must be represented numerically
            result += '&';
            result += '#';
            result += numberFormat->format(c.code());
            result += ';';
        }
        else
        {   //  "c" can be represented literally
            result += c;
        }
    }
    return result;
}

String Xml::decodeXmlEntities(const String & s)
{
    static _EntityRegistry * entityRegistry = _EntityRegistry::getInstance();   //  idempotent

    String result, entity;
    for (UIntSize i = 0; i < s.length(); )
    {
        Char c = s[i];
        if (c == '&')
        {   //  XML entity starts here, continuing until ';'
            entity.clear();
            while (i < s.length() && s[i] != ';')
            {
                entity += s[i++];
            }
            if (i < s.length())
            {   //  Must be the ';' closing the entity
                entity += s[i++];
            }
            if (entityRegistry->findCharForEntity(entity, c))
            {   //  Entity found
                result += c;
            }
            else if (entity.length() > 2 && entity[1] == '#' && entity[2].isDecimalDigit())
            {
                Char::Code code;
                UIntSize scan = 2;  //  start scanning after "&#"
                if (fromString(entity, scan, code) && scan == entity.length() - 1 && entity[scan] == ';')
                {
                    result += Char(code);
                }
                else
                {   //  OOPS! Malformed decimal entity reference
                    result += entity;
                }
            }
            //  TODO hexadecimal entities too
            else
            {   //  OOPS! Unknown or malformed entity reference
                result += entity;
            }
        }
        //  TODO numeric codes for e.g. CR, LF, etc.
        else
        {   //  Just a character
            result += c;
            i++;
        }
    }
    return result;
}

//////////
//  Xml::_EntityRegistry
CARL_IMPLEMENT_SINGLETON(Xml::_EntityRegistry)

Xml::_EntityRegistry::_EntityRegistry()
{
    _mapEntity("&lt;", '<');
    _mapEntity("&gt;", '>');
    _mapEntity("&quot;", '"');
    _mapEntity("&amp;", '&');
    _mapEntity("&apos;", '\'');
}

Xml::_EntityRegistry::~_EntityRegistry() noexcept
{
}

bool Xml::_EntityRegistry::findEntityForChar(Char c, String & entity) const
{
    if (_mapCharsToEntities.containsKey(c))
    {
        entity = _mapCharsToEntities[c];
        return true;
    }
    return false;
}

bool Xml::_EntityRegistry::findCharForEntity(const String & entity, Char & c) const
{
    if (_mapEntitiesToChars.containsKey(entity))
    {
        c = _mapEntitiesToChars[entity];
        return true;
    }
    return false;
}

void Xml::_EntityRegistry::_mapEntity(const String & entity, Char c)
{
    require(!_mapEntitiesToChars.containsKey(entity));
    require(!_mapCharsToEntities.containsKey(c));

    _mapEntitiesToChars[entity] = c;
    _mapCharsToEntities[c] = entity;
}

//  End of Xml.cpp
