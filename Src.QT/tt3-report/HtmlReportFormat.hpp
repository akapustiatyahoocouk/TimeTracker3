//
//  tt3-report/HtmlReportFormat.hpp - tt3-report HTML report format
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
    /// \class HtmlReportFormat tt3-report/API.hpp
    /// \brief A report format that writes a single HTML file.
    class TT3_REPORT_PUBLIC HtmlReportFormat
        :   public virtual IReportFormat
    {
        TT3_DECLARE_SINGLETON(HtmlReportFormat)

        //////////
        //  IReportFormat
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual auto    displayName() const -> QString override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;
        virtual QString preferredExtension() const override;
    };
}

//  End of tt3-report/HtmlReportFormat.hpp
