//
//  tt3-report/ReportBlockElement.cpp - tt3::report::ReportBlockElement class implementation
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
ReportBlockElement::ReportBlockElement(
        ReportFlowElement * parent
    ) : ReportElement(parent->report()),
        _parent(parent)
{
    //  Add to parent
    _parent->_children.append(this);
}

ReportBlockElement::~ReportBlockElement()
{
    //  Remove from parent
    Q_ASSERT(_parent->_children.contains(this));
    _parent->_children.removeAll(this);
}

//////////
//  Opertions
TypographicSize ReportBlockElement::resolveLeftMargin() const
{   //  Default behaviour is go to the parent block
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentBlock =
            dynamic_cast<const ReportBlockElement*>(parent))
        {
            return parentBlock->resolveLeftMargin();
        }
    }
    //  No ancestor block was any good
    return TypographicSize::pt(0);
}

TypographicSize ReportBlockElement::resolveRightMargin() const
{   //  Default behaviour is go to the parent block
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentBlock =
            dynamic_cast<const ReportBlockElement*>(parent))
        {
            return parentBlock->resolveRightMargin();
        }
    }
    //  No ancestor block was any good
    return TypographicSize::pt(0);
}

TypographicSize ReportBlockElement::resolveGapAboven() const
{   //  Default behaviour is go to the parent block
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentBlock =
            dynamic_cast<const ReportBlockElement*>(parent))
        {
            return parentBlock->resolveGapAboven();
        }
    }
    //  No ancestor block was any good
    return TypographicSize::pt(0);
}

TypographicSize ReportBlockElement::resolveGapBelow() const
{   //  Default behaviour is go to the parent block
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentBlock =
            dynamic_cast<const ReportBlockElement*>(parent))
        {
            return parentBlock->resolveGapBelow();
        }
    }
    //  No ancestor block was any good
    return TypographicSize::pt(0);
}

//////////
//  Serialization
void ReportBlockElement::serialize(QDomElement & element) const
{
    ReportElement::serialize(element);
}

void ReportBlockElement::deserialize(const QDomElement & element)
{
    ReportElement::deserialize(element);
}

//  End of tt3-report/ReportBlockElement.cpp

