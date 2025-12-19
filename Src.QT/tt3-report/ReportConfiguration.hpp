//
//  tt3-report/ReportConfiguration.hpp - tt3 report configuration ADT
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
    /// \class IReportConfiguration tt3-report/API.hpp
    /// \brief A configuration for a report generation job.
    class TT3_REPORT_PUBLIC IReportConfiguration
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IReportConfiguration)

        //////////
        //  This is an interface
    public:
        /// \brief
        ///     The class constructor.
        IReportConfiguration() = default;

        /// \brief
        ///     The class destructor.
        virtual ~IReportConfiguration() = default;
    };
}

//  End of tt3-report/ReportConfiguration.hpp
