//
//  tt3-report/ReportSpanElement.cpp - tt3::report::ReportSpanElement class implementation
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
ReportSpanElement::ReportSpanElement(
        ReportParagraph * paragraph
    ) : ReportElement(paragraph->_report),
        _paragraph(paragraph)
{
    //  Add to parent
    _paragraph->_children.append(this);
}

ReportSpanElement::~ReportSpanElement()
{
    delete _link;   //  "delete nullptr" is safe

    //  Remove from parent
    Q_ASSERT(_paragraph->_children.contains(this));
    _paragraph->_children.removeAll(this);
}

//////////
//  Operations
auto ReportSpanElement::createInternalLink(
        ReportAnchor * anchor,
        ILinkStyle * style
    ) -> ReportInternalLink *
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportInternalLink(this, style, anchor);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

auto ReportSpanElement::createExternalLink(
        const QString & url,
        ILinkStyle * style
    ) -> ReportExternalLink *
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportExternalLink(this, style, url);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//////////
//  Serialization
void ReportSpanElement::serialize(QDomElement & element) const
{
    ReportElement::serialize(element);

    if (_link != nullptr)
    {
        auto linkElement =
            element.ownerDocument().createElement(
                _link->xmlTagName());
        element.appendChild(linkElement);
        _link->serialize(linkElement);
    }
}

void ReportSpanElement::deserialize(const QDomElement & element)
{
    ReportElement::deserialize(element);

    for (QDomElement childElement = element.firstChildElement();
         !childElement.isNull();
         childElement = childElement.nextSiblingElement())
    {
        if (childElement.tagName() == ReportExternalLink::XmlTagName)
        {
            auto link = createExternalLink("");
            link->deserialize(childElement);
        }
        else if (childElement.tagName() == ReportInternalLink::XmlTagName)
        {   //  We cannot create an internal link now - the anchor may be
            //  defined later in the XML, or not al all. o, we instead
            //  record the Span -> AnchorID mapping fr future resolution
            QString anchorId = childElement.attribute("AnchorID");
            _report->_anchorIdsForInternalLinks[this] = anchorId;
            _report->_elementsForInternalLinks[this] = element;
        }
        //  There may be other children handled by base or derived classes!
    }
}

//  End of tt3-report/ReportSpanElement.cpp

