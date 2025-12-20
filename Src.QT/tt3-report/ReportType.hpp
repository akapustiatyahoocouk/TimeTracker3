//
//  tt3-report/ReportType.hpp - tt3 report types
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
    /// \class IReportType tt3-report/API.hpp
    /// \brief A generic "report type" is a factory for Reports.
    class TT3_REPORT_PUBLIC IReportType
    {
        //////////
        //  Types
    public:
        /// \brief
        ///     The agent notified of a report generation progress.
        using ProgressListener = std::function<void(float ratioCompleted)>;

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        IReportType() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~IReportType() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this ReportType.
        /// \return
        ///     The mnemonic identifier of this ReportType.
        virtual auto    mnemonic() const -> Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this ReportType.
        /// \return
        ///     The user-readable display name of this ReportType
        ///     for the current default locale.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this ReportType.
        /// \return
        ///     The short (1 line) description of this ReportType
        ///     for the current default locale.
        virtual QString description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this ReportType.
        /// \return
        ///     The small (16x16) icon representing this ReportType.
        virtual QIcon   smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this ReportType.
        /// \return
        ///     The large (32x32) icon representing this ReportType.
        virtual QIcon   largeIcon() const = 0;

        /// \brief
        ///     Creates a new configuration editor widget
        ///     for a report of this type.
        /// \param parent
        ///     The [initial] parent for the new widget; nullptr == none.
        /// \return
        ///     The newly created configuration editor widget
        ///     or nullptr if reports of this type have no
        ///     editable configuration.
        virtual auto    createConfigurationEditor(
                                QWidget * parent,
                                tt3::ws::Workspace workspace,
                                const tt3::ws::ReportCredentials & credentials
                            ) -> ReportConfigurationEditor * = 0;

        /// \brief
        ///     Generates the report.
        /// \param workspace
        ///     The workspace to report from.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param configuration
        ///     The report configuration; nullptr == default or none.
        /// \param reportTemplate
        ///     The template for the report.
        /// \param progressListener
        ///     The callback to use for save progress notification,
        ///     nullptr == don't notify.
        /// \return
        ///     The generated report.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        /// \exception ReportException
        ///     If a report generation error occurs.
        ///     This specifically includes the case when
        ///     the specified "configuration" is incompatible
        ///     with this report type.
        virtual Report *generateReport(
                                const tt3::ws::Workspace & workspace,
                                const tt3::ws::ReportCredentials & credentials,
                                const IReportConfiguration * configuration,
                                const IReportTemplate * reportTemplate,
                                ProgressListener progressListener
                            ) = 0;
    };

    /// \class ReportTypeManager tt3-report/API.hpp
    /// \brief The manager of known ReportTypes.
    class TT3_REPORT_PUBLIC ReportTypeManager final
    {
        TT3_UTILITY_CLASS(ReportTypeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered ReportTypes.
        /// \return
        ///     The set of all registered ReportTypes.
        static auto     allReportTypes() -> ReportTypes;

        /// \brief
        ///     Registers the specified ReportType.
        /// \param reportType
        ///     The ReportType to register,
        /// \return
        ///     True on success, false on failure. A repeated
        ///     registration of the same ReportType is treated as a "success".
        static bool     registerReportType(IReportType * reportType);

        /// \brief
        ///     Un-registers the specified ReportType.
        /// \param reportType
        ///     The ReportType to un-register,
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterReportType(IReportType * reportType);

        /// \brief
        ///     Finds a registered ReportType by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered ReportType with the required mnemonic;
        ///     nullptr if not found.
        static auto     findReportType(
                                const tt3::util::Mnemonic & mnemonic
                            ) -> IReportType *;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-report/ReportType.hpp
