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

auto IReportTemplate::findCharacterStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> ICharacterStyle *
{
    return dynamic_cast<ICharacterStyle*>(findStyleByMnemonic(mnemonic));
}

auto IReportTemplate::findBlockStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> IBlockStyle *
{
    return dynamic_cast<IBlockStyle*>(findStyleByMnemonic(mnemonic));
}

auto IReportTemplate::findParagraphStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> IParagraphStyle *
{
    return dynamic_cast<IParagraphStyle*>(findStyleByMnemonic(mnemonic));
}

auto IReportTemplate::findListStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> IListStyle *
{
    return dynamic_cast<IListStyle*>(findStyleByMnemonic(mnemonic));
}

auto IReportTemplate::findTableStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> ITableStyle *
{
    return dynamic_cast<ITableStyle*>(findStyleByMnemonic(mnemonic));
}

auto IReportTemplate::findLinkStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> ILinkStyle *
{
    return dynamic_cast<ILinkStyle*>(findStyleByMnemonic(mnemonic));
}

auto IReportTemplate::findSectionStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> ISectionStyle *
{
    return dynamic_cast<ISectionStyle*>(findStyleByMnemonic(mnemonic));
}

QDomDocument IReportTemplate::toXmlDocument() const
{
    //  Create DOM document
    QDomDocument document;
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='yes'");
    document.appendChild(xmlDeclaration);

    //  TODO
    //  DONE
    return document;
}

QString IReportTemplate::toXmlString() const
{
    QDomDocument document = toXmlDocument();
    QString xmlString;
    QTextStream stream(&xmlString);
    document.save(stream, 4);    //  4 spaces per indent level
    return xmlString;
}

//  End of tt3-report/IReportTemplate.cpp
