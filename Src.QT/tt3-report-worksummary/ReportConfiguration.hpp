//
//  tt3-report-worksummary/ReportConfiguration.hpp - The Work Summary report configuration
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

namespace tt3::report::worksummary
{
    /// \class ReportConfiguration tt3-report-worksummary/API.hpp
    /// \brief The "Work Summary" report configuration.
    class TT3_REPORT_WORKSUMMARY_PUBLIC ReportConfiguration final
        :   public tt3::report::ReportConfiguration
    {
        //////////
        //  Construction/destruction/assignment
    public:
        ReportConfiguration();
        virtual ~ReportConfiguration();
    };
}

//  End of tt3-report-worksummary/ReportType.hpp
