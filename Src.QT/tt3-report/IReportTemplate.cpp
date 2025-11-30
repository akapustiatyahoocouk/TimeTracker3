//
//  tt3-report/IReportTemplate.cpp - tt3::report::IReportTemplate class implementation
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
#include "tt3-report/API.hpp"
using namespace tt3::report;

//////////
//  Operations
auto IReportTemplate::characterStyles() const -> CharacterStyles
{
    CharacterStyles result;
    for (auto style : styles())
    {
        if (auto characterStyle = dynamic_cast<ICharacterStyle*>(style))
        {
            result.insert(characterStyle);
        }
    }
    return result;
}

auto IReportTemplate::blockStyles() const -> BlockStyles
{
    BlockStyles result;
    for (auto style : styles())
    {
        if (auto blockStyle = dynamic_cast<IBlockStyle*>(style))
        {
            result.insert(blockStyle);
        }
    }
    return result;
}

auto IReportTemplate::paragraphStyles() const -> ParagraphStyles
{
    ParagraphStyles result;
    for (auto style : styles())
    {
        if (auto paragraphStyle = dynamic_cast<IParagraphStyle*>(style))
        {
            result.insert(paragraphStyle);
        }
    }
    return result;
}

auto IReportTemplate::listStyles() const -> ListStyles
{
    ListStyles result;
    for (auto style : styles())
    {
        if (auto listStyle = dynamic_cast<IListStyle*>(style))
        {
            result.insert(listStyle);
        }
    }
    return result;
}

auto IReportTemplate::tableStyles() const -> TableStyles
{
    TableStyles result;
    for (auto style : styles())
    {
        if (auto tableStyle = dynamic_cast<ITableStyle*>(style))
        {
            result.insert(tableStyle);
        }
    }
    return result;
}

auto IReportTemplate::linkStyles() const -> LinkStyles
{
    LinkStyles result;
    for (auto style : styles())
    {
        if (auto linkStyle = dynamic_cast<ILinkStyle*>(style))
        {
            result.insert(linkStyle);
        }
    }
    return result;
}

auto IReportTemplate::sectionStyles() const -> SectionStyles
{
    SectionStyles result;
    for (auto style : styles())
    {
        if (auto sectionStyle = dynamic_cast<ISectionStyle*>(style))
        {
            result.insert(sectionStyle);
        }
    }
    return result;
}

auto IReportTemplate::findCharacterStyleByName(
        const QString & name
    ) const -> ICharacterStyle *
{
    return dynamic_cast<ICharacterStyle*>(findStyleByName(name));
}

auto IReportTemplate::findBlockStyleByName(
        const QString & name
    ) const -> IBlockStyle *
{
    return dynamic_cast<IBlockStyle*>(findStyleByName(name));
}

auto IReportTemplate::findParagraphStyleByName(
        const QString & name
    ) const -> IParagraphStyle *
{
    return dynamic_cast<IParagraphStyle*>(findStyleByName(name));
}

auto IReportTemplate::findListStyleByName(
        const QString & name
    ) const -> IListStyle *
{
    return dynamic_cast<IListStyle*>(findStyleByName(name));
}

auto IReportTemplate::findTableStyleByName(
        const QString & name
    ) const -> ITableStyle *
{
    return dynamic_cast<ITableStyle*>(findStyleByName(name));
}

auto IReportTemplate::findLinkStyleByName(
        const QString & name
    ) const -> ILinkStyle *
{
    return dynamic_cast<ILinkStyle*>(findStyleByName(name));
}

auto IReportTemplate::findSectionStyleByName(
        const QString & name
    ) const -> ISectionStyle *
{
    return dynamic_cast<ISectionStyle*>(findStyleByName(name));
}

//  End of tt3-report/IReportTemplate.cpp
