//
//  tt3-report/ReportParagraph.cpp - tt3::report::ReportParagraph class implementation
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
//  Construction/destruction - from friends only
ReportParagraph::ReportParagraph(
        ReportFlowElement * parent,
        IParagraphStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportParagraph::~ReportParagraph()
{
    for (auto child : ReportSpanElements(_children))    //  shallow clone
    {
        delete child;   //  removes SpanElement from Paragraph
    }
}

//////////
//  ReportElement
auto ReportParagraph::resolveFontSpecs() const -> FontSpecs
{
    if (_style != nullptr)
    {
        auto styleFontSpecs = _style->fontSpecs();
        if (!styleFontSpecs.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontSpecs();
        }
        else if (styleFontSpecs.value().isEmpty())
        {   //  Go directly to template
            return _report->_reportTemplate->defaultFontSpecs();
        }
        else
        {   //  Use value from style
            return styleFontSpecs.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSpecs();
}

auto ReportParagraph::resolveFontSize() const -> TypographicSize
{
    if (_style != nullptr)
    {
        auto styleFontSize = _style->fontSize();
        if (!styleFontSize.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontSize();
        }
        else
        {   //  Use value from style
            return styleFontSize.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSize();
}

auto ReportParagraph::resolveFontStyle() const -> FontStyle
{
    if (_style != nullptr)
    {
        auto styleFontStyle = _style->fontStyle();
        if (!styleFontStyle.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontStyle();
        }
        else
        {   //  Use value from style
            return styleFontStyle.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontStyle();
}

auto ReportParagraph::resolveTextColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleTextColor = _style->textColor();
        if (!styleTextColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveTextColor();
        }
        else if (styleTextColor.value() == ColorSpec::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultTextColor();
        }
        else
        {   //  Use value from style
            return styleTextColor.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveTextColor();
}

auto ReportParagraph::resolveBackgroundColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleBackgroundColor = _style->backgroundColor();
        if (!styleBackgroundColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveBackgroundColor();
        }
        else if (styleBackgroundColor.value() == ColorSpec::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultBackgroundColor();
        }
        else
        {   //  Use value from style
            return styleBackgroundColor.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveBackgroundColor();
}

//////////
//  ReportBlockElement
TypographicSize ReportParagraph::resolveLeftMargin() const
{
    //  Honor own style first
    if (_style != nullptr && _style->leftMargin().has_value())
    {
        return _style->leftMargin().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveLeftMargin();
}

TypographicSize ReportParagraph::resolveRightMargin() const
{
    //  Honor own style first
    if (_style != nullptr && _style->rightMargin().has_value())
    {
        return _style->rightMargin().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveRightMargin();
}

TypographicSize ReportParagraph::resolveGapAboven() const
{
    //  Honor own style first
    if (_style != nullptr && _style->gapAbove().has_value())
    {
        return _style->gapAbove().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveGapAboven();
}

TypographicSize ReportParagraph::resolveGapBelow() const
{
    //  Honor own style first
    if (_style != nullptr && _style->gapBelow().has_value())
    {
        return _style->gapBelow().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveGapBelow();
}

//////////
//  Operations
void ReportParagraph::setStyle(IParagraphStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    if (style == nullptr ||
        style->reportTemplate() == _report->reportTemplate())
    {   //  Be defensive in Release mode
        _style = style;
    }
}

auto ReportParagraph::children() -> ReportSpanElements
{
    return _children;
}

auto ReportParagraph::children() const -> ReportSpanElementsC
{
    return ReportSpanElementsC(_children.cbegin(), _children.cend());
}

auto ReportParagraph::resolveTextAlignment() const-> HorizontalAlignment
{
    //  Honor own style first
    if (_style != nullptr && _style->textAlignment().has_value())
    {
        return _style->textAlignment().value();
    }
    //  Must go to the parent paragraph
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentParagraph =
            dynamic_cast<const ReportParagraph*>(parent))
        {
            return parentParagraph->resolveTextAlignment();
        }
    }
    //  No ancestor paragraph was any good
    return HorizontalAlignment::Default;
}

BorderType ReportParagraph::resolveBorderType() const
{
    //  Honor own style first
    if (_style != nullptr && _style->borderType().has_value())
    {
        return _style->borderType().value();
    }
    //  Must go to the parent paragraph
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentParagraph =
            dynamic_cast<const ReportParagraph*>(parent))
        {
            return parentParagraph->resolveBorderType();
        }
    }
    //  No ancestor paragraph was any good
    return BorderType::Default;
}

ReportText * ReportParagraph::createText(
        const QString & text,
        ICharacterStyle * style
    )
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    auto result = new ReportText(this, text, style);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

ReportPicture * ReportParagraph::createPicture(
        const TypographicSize & width,
        const TypographicSize & height,
        const QImage & image
    )
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportPicture(this, width, height, image);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//////////
//  Serialization
void ReportParagraph::serialize(QDomElement & element) const
{
    ReportBlockElement::serialize(element);

    if (_style != nullptr)
    {
        element.setAttribute("Style", _style->name().toString());
    }
    for (auto child : _children)
    {
        auto childElement =
            element.ownerDocument().createElement(
                child->xmlTagName());
        element.appendChild(childElement);
        child->serialize(childElement);
    }
}

//  End of tt3-report/ReportParagraph.cpp

