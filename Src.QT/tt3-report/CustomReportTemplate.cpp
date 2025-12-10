//
//  tt3-report/CustomReportTemplate.cpp - tt3::report::CustomReportTemplate class implementation
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
//  Construction/destruction
CustomReportTemplate::CustomReportTemplate(
        const QString & xml
    ) : _pageSetup(),   //  default, A4
        //  start with defaults
        _defaultFontSpecs{FontSpec::SansSerif},
        _defaultFontSize(TypographicSize::pt(12)),
        _defaultFontStyle(FontStyle::Plain),
        _defaultTextColor(ColorSpec::Default),
        _defaultBackgroundColor(ColorSpec::Default),
        _defaultListIndent(TypographicSize::in(0.25)),
        _defaultTableBorderType(BorderType::Double),
        _defaultCellBorderType(BorderType::Single),
        _defaultLinkUnderlineMode(UnderlineMode::Single),
        _defaultPageNumberPlacement(PageNumberPlacement::Default)
{
    QDomDocument document;

    auto parseResult = document.setContent(xml);
    if (!parseResult)
    {   //  OOPS! Throw a proper exception!
        throw CustomReportException(parseResult.errorMessage);
    }
    _construct(document);
}

CustomReportTemplate::CustomReportTemplate(
        const QDomDocument & document
    ) : _pageSetup(),   //  default, A4
        //  start with defaults
        _defaultFontSpecs{FontSpec::SansSerif},
        _defaultFontSize(TypographicSize::pt(12)),
        _defaultFontStyle(FontStyle::Plain),
        _defaultTextColor(ColorSpec::Default),
        _defaultBackgroundColor(ColorSpec::Default),
        _defaultListIndent(TypographicSize::in(0.25)),
        _defaultTableBorderType(BorderType::Double),
        _defaultCellBorderType(BorderType::Single),
        _defaultLinkUnderlineMode(UnderlineMode::Single),
        _defaultPageNumberPlacement(PageNumberPlacement::Default)
{
    _construct(document);
}

CustomReportTemplate::~CustomReportTemplate()
{
    Q_ASSERT(_referenceCount == 0);
    for (auto style : _styles.values())
    {
        delete style;
    }
}

//////////
//  IReportTemplate
auto CustomReportTemplate::mnemonic() const -> Mnemonic
{
    return _mnemonic;
}

auto CustomReportTemplate::displayName() const -> QString
{
    return _displayName;
}

auto CustomReportTemplate::pageSetup() const -> PageSetup
{
    return _pageSetup;
}

auto CustomReportTemplate::defaultFontSpecs() const -> FontSpecs
{
    return _defaultFontSpecs;
}

auto CustomReportTemplate::defaultFontSize() const -> TypographicSize
{
    return _defaultFontSize;
}

auto CustomReportTemplate::defaultFontStyle() const -> FontStyle
{
    return _defaultFontStyle;
}

auto CustomReportTemplate::defaultTextColor() const -> ColorSpec
{
    return _defaultTextColor;
}

auto CustomReportTemplate::defaultBackgroundColor() const -> ColorSpec
{
    return _defaultBackgroundColor;
}

auto CustomReportTemplate::defaultListIndent() const -> TypographicSize
{
    return _defaultListIndent;
}

auto CustomReportTemplate::defaultTableBorderType() const -> BorderType
{
    return _defaultTableBorderType;
}

auto CustomReportTemplate::defaultCellBorderType() const -> BorderType
{
    return _defaultCellBorderType;
}

auto CustomReportTemplate::defaultLinkUnderlineMode() const -> UnderlineMode
{
    return _defaultLinkUnderlineMode;
}

auto CustomReportTemplate::defaultPageNumberPlacement() const -> PageNumberPlacement
{
    return _defaultPageNumberPlacement;
}

auto CustomReportTemplate::styles() const -> Styles
{
    QList<CustomStyle*> styles = _styles.values();
    return Styles(styles.cbegin(), styles.cend());
}

auto CustomReportTemplate::findStyleByName(
        const Mnemonic & name
    ) const -> IStyle *
{
    return _styles.contains(name) ? _styles[name] : nullptr;
}

//////////
//  Implementation helpers
void CustomReportTemplate::_construct(
        const QDomDocument & document
    )
{
    //  Locate & validate the root element
    if (document.documentElement().isNull() ||
        document.documentElement().tagName() != "CustomReportTemplate" ||
        document.documentElement().attribute("FormatVersion") != "1")
    {   //  OOPS! Not a valid template!
        throw InvalidReportTemplateException();
    }

    //  Locate the "Properties" element and parse report
    //  template properties - we can use data members directly
    QDomElement propertiesElement = _getChildElement(document.documentElement(), "Properties");

    _parseAttribute(
        propertiesElement,
        "Mnemonic",
        _mnemonic);
    _parseAttribute(
        propertiesElement,
        "DisplayName",
        _displayName);
    _parseAttribute(
        propertiesElement,
        "PageSetup",
        _pageSetup);
    _parseAttribute(
        propertiesElement,
        "DefaultFontSpecs",
        _defaultFontSpecs);
    _parseAttribute(
        propertiesElement,
        "DefaultFontSize",
        _defaultFontSize);
    _parseAttribute(
        propertiesElement,
        "DefaultFontStyle",
        _defaultFontStyle);
    _parseAttribute(
        propertiesElement,
        "DefaultTextColor",
        _defaultTextColor);
    _parseAttribute(
        propertiesElement,
        "DefaultBackgroundColor",
        _defaultBackgroundColor);
    _parseAttribute(
        propertiesElement,
        "DefaultListIndent",
        _defaultListIndent);
    _parseAttribute(
        propertiesElement,
        "DefaultTableBorderType",
        _defaultTableBorderType);
    _parseAttribute(
        propertiesElement,
        "DefaultCellBorderType",
        _defaultCellBorderType);
    _parseAttribute(
        propertiesElement,
        "DefaultLinkUnderlineMode",
        _defaultLinkUnderlineMode);
    _parseAttribute(
        propertiesElement,
        "DefaultPageNumberPlacement",
        _defaultPageNumberPlacement);

    //  Do the styles
    QDomElement stylesElement = _getChildElement(document.documentElement(), "Styles");
    for (auto styleElement = stylesElement.firstChildElement();
         !styleElement.isNull();
         styleElement = styleElement.nextSiblingElement())
    {
        CustomStyle * style;
        if (styleElement.tagName() == IParagraphStyle::_XmlTagName)
        {
            style = new CustomParagraphStyle(this);
        }
        else if (styleElement.tagName() == IListStyle::_XmlTagName)
        {
            style = new CustomListStyle(this);
        }
        else if (styleElement.tagName() == ITableStyle::_XmlTagName)
        {
            style = new CustomTableStyle(this);
        }
        else if (styleElement.tagName() == ILinkStyle::_XmlTagName)
        {
            style = new CustomLinkStyle(this);
        }
        else if (styleElement.tagName() == ISectionStyle::_XmlTagName)
        {
            style = new CustomSectionStyle(this);
        }
        else
        {   //  OOPS!
            throw InvalidReportTemplateException();
        }
        style->_deserialize(styleElement);
        _addStyle(style);
    }

    //  Done
    validate();
}

void CustomReportTemplate::_addStyle(CustomStyle * style)
{
    Q_ASSERT(style != nullptr);

    if (_styles.contains(style->_name))
    {   //  OOPS! Duplicate styles in report template XML!
        throw InvalidReportTemplateException();
    }
    _styles[style->_name] = style;
}

QDomElement CustomReportTemplate::_findChildElement(
        const QDomElement & parentElement,
        const QString & childTagName
    )
{
    QDomElement result;
    for (auto childElement = parentElement.firstChildElement();
         !childElement.isNull();
         childElement = childElement.nextSiblingElement())
    {
        if (childElement.tagName() == childTagName)
        {   //  Record & keep looking
            if (!result.isNull())
            {   //  OOPS! Duplicate!
                throw InvalidReportTemplateException();
            }
            result = childElement;
        }
    }
    return result;  //  my be a "null" element
}

QDomElement CustomReportTemplate::_getChildElement(
        const QDomElement & parentElement,
        const QString & childTagName
    )
{
    QDomElement result;
    for (auto childElement = parentElement.firstChildElement();
         !childElement.isNull();
         childElement = childElement.nextSiblingElement())
    {
        if (childElement.tagName() == childTagName)
        {   //  Record & keep looking
            if (!result.isNull())
            {   //  OOPS! Duplicate!
                throw InvalidReportTemplateException();
            }
            result = childElement;
        }
    }
    if (result.isNull())
    {   //  OOPS! Not found!
        throw InvalidReportTemplateException();
    }
    return result;
}

//  End of tt3-report/CustomReportTemplate.cpp
