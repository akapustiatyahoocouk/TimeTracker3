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
QIcon IReportTemplate::smallIcon() const
{
    static const QIcon icon(":/tt3-report/Resources/Images/Reports/ReportTemplateSmall.png");
    return icon;
}

QIcon IReportTemplate::largeIcon() const
{
    static const QIcon icon(":/tt3-report/Resources/Images/Reports/ReportTemplateLarge.png");
    return icon;
}

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

auto IReportTemplate::style(
        const Mnemonic & name
    ) const -> IStyle *
{
    if (auto result = findStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}

auto IReportTemplate::findCharacterStyle(
        const Mnemonic & name
    ) const -> ICharacterStyle *
{
    return dynamic_cast<ICharacterStyle*>(findStyle(name));
}

auto IReportTemplate::characterStyle(
        const Mnemonic & name
    ) const -> ICharacterStyle *
{
    if (auto result = findCharacterStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}

auto IReportTemplate::findBlockStyle(
        const Mnemonic & name
    ) const -> IBlockStyle *
{
    return dynamic_cast<IBlockStyle*>(findStyle(name));
}

auto IReportTemplate::blockStyle(
        const Mnemonic & name
    ) const -> IBlockStyle *
{
    if (auto result = findBlockStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}

auto IReportTemplate::findParagraphStyle(
        const Mnemonic & name
    ) const -> IParagraphStyle *
{
    return dynamic_cast<IParagraphStyle*>(findStyle(name));
}

auto IReportTemplate::paragraphStyle(
        const Mnemonic & name
    ) const -> IParagraphStyle *
{
    if (auto result = findParagraphStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}

auto IReportTemplate::findListStyle(
        const Mnemonic & name
    ) const -> IListStyle *
{
    return dynamic_cast<IListStyle*>(findStyle(name));
}

auto IReportTemplate::listStyle(
        const Mnemonic & name
    ) const -> IListStyle *
{
    if (auto result = findListStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}

auto IReportTemplate::findTableStyle(
        const Mnemonic & name
    ) const -> ITableStyle *
{
    return dynamic_cast<ITableStyle*>(findStyle(name));
}

auto IReportTemplate::tableStyle(
        const Mnemonic & name
    ) const -> ITableStyle *
{
    if (auto result = findTableStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}


auto IReportTemplate::findLinkStyle(
        const Mnemonic & name
    ) const -> ILinkStyle *
{
    return dynamic_cast<ILinkStyle*>(findStyle(name));
}

auto IReportTemplate::linkStyle(
    const Mnemonic & name
    ) const -> ILinkStyle *
{
    if (auto result = findLinkStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}

auto IReportTemplate::findSectionStyle(
        const Mnemonic & name
    ) const -> ISectionStyle *
{
    return dynamic_cast<ISectionStyle*>(findStyle(name));
}

auto IReportTemplate::sectionStyle(
        const Mnemonic & name
    ) const -> ISectionStyle *
{
    if (auto result = findSectionStyle(name))
    {
        return result;
    }
    throw StyleDoesNotExistException(name, mnemonic(), displayName());
}
QDomDocument IReportTemplate::toXmlDocument() const
{
    //  Create DOM document
    QDomDocument document;
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='yes'");
    document.appendChild(xmlDeclaration);

    //  Set up the root element
    QDomElement rootElement = document.createElement(XmlTagName);
    document.appendChild(rootElement);

    //  Go!
    serialize(rootElement);

    //  Done
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

void IReportTemplate::validate() const
{
#define CHECK_STYLE(Type, Name)                         \
    if (find##Type##Style(                              \
            I##Type##Style::Name##StyleName) == nullptr)\
    {                                                   \
        qCritical() <<                                  \
            "Report template " +                        \
            this->displayName() +                       \
            " (" +                                      \
            this->mnemonic().toString() +               \
            ") does not define style " +                \
            "I" + #Type + "Style::" +                   \
            #Name + "StyleName";                        \
        throw InvalidReportTemplateException();         \
    }

    CHECK_STYLE(Paragraph, Title)
    CHECK_STYLE(Paragraph, Subtitle)
    CHECK_STYLE(Paragraph, Default)
    CHECK_STYLE(Paragraph, Heading1)
    CHECK_STYLE(Paragraph, Heading2)
    CHECK_STYLE(Paragraph, Heading3)
    CHECK_STYLE(Paragraph, Heading4)
    CHECK_STYLE(Paragraph, Heading5)
    CHECK_STYLE(Paragraph, Heading6)
    CHECK_STYLE(Paragraph, Heading7)
    CHECK_STYLE(Paragraph, Heading8)
    CHECK_STYLE(Paragraph, Heading9)
    CHECK_STYLE(Paragraph, Toc1)
    CHECK_STYLE(Paragraph, Toc2)
    CHECK_STYLE(Paragraph, Toc3)
    CHECK_STYLE(Paragraph, Toc4)
    CHECK_STYLE(Paragraph, Toc5)
    CHECK_STYLE(Paragraph, Toc6)
    CHECK_STYLE(Paragraph, Toc7)
    CHECK_STYLE(Paragraph, Toc8)
    CHECK_STYLE(Paragraph, Toc9)

    CHECK_STYLE(List, Default)
    CHECK_STYLE(Table, Default)
    CHECK_STYLE(Link, Default)

    CHECK_STYLE(Section, Title)
    CHECK_STYLE(Section, Prequel)
    CHECK_STYLE(Section, Body)
    CHECK_STYLE(Section, Sequel)

#undef CHECK_STYLE
}

//////////
//  Serialization
void IReportTemplate::serialize(QDomElement & element) const
{
    Q_ASSERT(!element.isNull());
    Q_ASSERT(!element.ownerDocument().isNull());

    //  Set report template element's attributes
    element.setAttribute("FormatVersion", FormatVersion);

    //  Set up "template properties" element
    QDomElement propertiesElement = element.ownerDocument().createElement("Properties");
    element.appendChild(propertiesElement);

    _setAttribute(propertiesElement, "Mnemonic", this->mnemonic());
    _setAttribute(propertiesElement, "DisplayName", this->displayName());   //  this is for "default" display name
    _setAttribute(propertiesElement, "PageSetup", this->pageSetup());
    _setAttribute(propertiesElement, "DefaultFontSpecs", this->defaultFontSpecs());
    _setAttribute(propertiesElement, "DefaultFontSize", this->defaultFontSize());
    _setAttribute(propertiesElement, "DefaultFontStyle", this->defaultFontStyle());
    _setAttribute(propertiesElement, "DefaultTextColor", this->defaultTextColor());
    _setAttribute(propertiesElement, "DefaultBackgroundColor", this->defaultBackgroundColor());
    _setAttribute(propertiesElement, "DefaultListIndent", this->defaultListIndent());
    _setAttribute(propertiesElement, "DefaultTableBorderType", this->defaultTableBorderType());
    _setAttribute(propertiesElement, "DefaultCellBorderType", this->defaultCellBorderType());
    _setAttribute(propertiesElement, "DefaultLinkUnderlineMode", this->defaultLinkUnderlineMode());
    _setAttribute(propertiesElement, "DefaultPageNumberPlacement", this->defaultPageNumberPlacement());

    //  Set up "styles" element
    QDomElement stylesElement =  element.ownerDocument().createElement("Styles");
    element.appendChild(stylesElement);

    //  Keep styles sorted by name to minimize the diffs
    //  between the report template XML files
    Styles styles = this->styles();
    QList<IStyle*> stylesList(styles.cbegin(), styles.cend());
    std::sort(
        stylesList.begin(),
        stylesList.end(),
        [](auto a, auto b)
        {
            return a->name() < b->name();
        });
    for (IStyle * style : stylesList)
    {
        QDomElement styleElement =  element.ownerDocument().createElement(style->xmlTagName());
        stylesElement.appendChild(styleElement);
        style->serialize(styleElement);
    }

    //  Serialize empty report
    std::unique_ptr<Report> report
        { createNewReport() };
    QDomElement reportElement = element.ownerDocument().createElement("Report");
    element.appendChild(reportElement);
    report->serialize(reportElement);
}

//  End of tt3-report/IReportTemplate.cpp
