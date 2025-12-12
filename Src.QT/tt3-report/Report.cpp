//
//  tt3-report/Report.cpp - tt3::report::Report class implementation
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
Report::Report(
        const QString & name,
        const IReportTemplate * reportTemplate
    ) : _name(name),
        _reportTemplate(reportTemplate),
        _createdAt(QDateTime::currentDateTimeUtc())
{
    Q_ASSERT(_reportTemplate != nullptr);

    _reportTemplate->_referenceCount++;
}

Report::~Report()
{
    clear();
    _reportTemplate->_referenceCount--;
}

//////////
//  Operations
void Report::clear()
{
#ifdef QT_DEBUG
    _validate();
#endif
    for (auto link : ReportLinks(_links))  //  shallow clone
    {
        delete link; //  removes Links from Report ans SpanElements
    }
    for (auto anchor : ReportAnchors(_anchors))  //  shallow clone
    {
        delete anchor; //  removes Anchors from Report ans Elements
    }
    for (auto section : ReportSections(_sections))  //  shallow clone
    {
        delete section; //  removes Sections from Report
    }
#ifdef QT_DEBUG
    _validate();
#endif
}

ReportSections Report::sections()
{
    return _sections;
}

ReportSectionsC Report::sections() const
{
    return ReportSectionsC(_sections.cbegin(), _sections.cend());
}

ReportAnchors Report::anchors()
{
    return _anchors;
}

ReportAnchorsC Report::anchors() const
{
    return ReportAnchorsC(_anchors.cbegin(), _anchors.cend());
}

ReportLinks Report::links()
{
    return _links;
}

ReportLinksC Report::links() const
{
    return ReportLinksC(_links.cbegin(), _links.cend());
}

ReportSection * Report::createSection(
        const QString & name,
        ISectionStyle * style
    )
{
#ifdef QT_DEBUG
    _validate();
#endif
    auto result = new ReportSection(this, name, style);
#ifdef QT_DEBUG
    _validate();
#endif
    return result;
}

//////////
//  Serialization
void Report::serialize(QDomElement & element) const
{
    Q_ASSERT(!element.isNull());

    //  Report properties
    element.setAttribute("FormatVersion", FormatVersion);
    element.setAttribute("Template", _reportTemplate->mnemonic().toString());
    element.setAttribute("Name", _name);
    element.setAttribute("CreatedAt", tt3::util::toString(_createdAt));

    //  Report content - it's sufficient to serialize
    //  _sections, as _anchors and _links will be re-created
    //  when a Report is deserialized
    for (auto section : _sections)
    {
        auto sectionElement =
            element.ownerDocument().createElement(
                section->xmlTagName());
        element.appendChild(sectionElement);
        section->serialize(sectionElement);
    }
}

void Report::deserialize(const QDomElement & element)
{
    Q_ASSERT(!element.isNull());

    //  Report properties
    if (element.attribute("FormatVersion") != FormatVersion)
    {   //  OOPS! Format version mismatch
        qCritical() << "FormatVersion != " + FormatVersion;
        throw InvalidReportException();
    }
    if (element.attribute("Template") != _reportTemplate->mnemonic().toString())
    {   //  OOPS! Template mismatch!
        qCritical() << "Template != " + _reportTemplate->mnemonic().toString();
        throw InvalidReportException();
    }
    _name = element.attribute("Name");
    _createdAt =
        tt3::util::fromString<QDateTime>(
            element.attribute("CreatedAt"),
            _createdAt);    //  default == no change

    //  We're re-creating the entire content of a report
    clear();
}

#ifdef QT_DEBUG
//////////
//  Validation
void Report::_validate()
{
    //  Validate back links
    for (auto section : _sections)
    {
        Q_ASSERT(section->_report == this);
    }
    for (auto anchor : _anchors)
    {
        Q_ASSERT(anchor->_report == this);
    }
    for (auto link : _links)
    {
        Q_ASSERT(link->_report == this);
    }

    //  Validate objects
    //  TODO
}
#endif

//  End of tt3-report/Report.cpp
