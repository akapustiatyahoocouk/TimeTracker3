//
//  tt3-report-worksummary/ReportType.hpp - The Work Summary report type
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
    /// \class ReportType tt3-report-worksummary/API.hpp
    /// \brief The "Work Summary" report type.
    class TT3_REPORT_WORKSUMMARY_PUBLIC ReportType final
    :   public virtual tt3::report::IReportType
    {
        TT3_DECLARE_SINGLETON(ReportType)

        //////////
        //  IReportType
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual QString displayName() const override;
        virtual QString description() const override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;
        virtual auto    createConfigurationEditor(
                                QWidget * parent
                            ) -> ReportConfigurationEditor * override;
        virtual auto    generateReport(
                                const tt3::ws::Workspace & workspace,
                                const tt3::ws::ReportCredentials & credentials,
                                const tt3::report::IReportConfiguration * configuration,
                                const tt3::report::IReportTemplate * reportTemplate,
                                ProgressListener progressListener
                            ) -> tt3::report::Report * override;
    };
}

//  End of tt3-report-worksummary/ReportType.hpp
