//
//  tt3-report/ReportTemplate.hpp - tt3-report template API
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

namespace tt3::report
{
    /// \class IReportTemplate tt3-report/API.hpp
    /// \brief An abstract template that can be chosen to format a printed report.
    class TT3_REPORT_PUBLIC IReportTemplate
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IReportTemplate() = default;

        /// \brief
        ///     The default [interface] destructor.
        virtual ~IReportTemplate() = default;
    };
}

//  End of tt3-report/ReportTemplate.hpp
