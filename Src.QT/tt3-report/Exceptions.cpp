//
//  tt3-report/Exceptions.cpp - Reporting exceptions
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
//  ReportException
ReportException::ReportException()
{
}

//////////
//  InvalidReportTemplateException
InvalidReportTemplateException::InvalidReportTemplateException()
{
}

QString InvalidReportTemplateException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(InvalidReportTemplateException));
}

//////////
//  ReportTemplateAlreadyExistsException
ReportTemplateAlreadyExistsException::ReportTemplateAlreadyExistsException(
        const tt3::util::Mnemonic & mnemonic,
        const QString & displayName
    ) : _mnemonic(mnemonic),
        _displayName(displayName)
{
}

QString ReportTemplateAlreadyExistsException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(ReportTemplateAlreadyExistsException),
        _displayName,
        _mnemonic);
}

//////////
//  CustomReportException
CustomReportException::CustomReportException(
        const QString & errorMessage
    ) : _errorMessage(errorMessage)
{
}

QString CustomReportException::errorMessage() const
{
    return _errorMessage;
}

//  End of tt3-report/Exceptions.cpp
