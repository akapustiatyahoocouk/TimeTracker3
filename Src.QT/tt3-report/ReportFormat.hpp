//
//  tt3-report/ReportFormat.hpp - tt3-report format API
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
    /// \class IReportFormat tt3-report/API.hpp
    /// \brief An abstract format in which a report can be saved.
    class TT3_REPORT_PUBLIC IReportFormat
    {
        //////////
        //  Types
    public:
        /// \brief
        ///     The agent notified of a save progress.
        using ProgressListener = std::function<void(double ratioCompleted)>;

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IReportFormat() = default;

        /// \brief
        ///     The default [interface] destructor.
        virtual ~IReportFormat() = default;

        //////////
        //  Operations
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this report format.
        /// \return
        ///     The mnemonic identifier of this report format.
        virtual auto    mnemonic() const -> Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this report format.
        /// \return
        ///     The user-readable display name of this report format
        ///     for the current default locale.
        virtual auto    displayName() const -> QString = 0;

        /// \brief
        ///     Returns the short (1 line) description of this ReportFormat.
        /// \return
        ///     The short (1 line) description of this ReportFormat
        ///     for the current default locale.
        virtual QString description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this ReportFormat.
        /// \return
        ///     The small (16x16) icon representing this ReportFormat.
        virtual QIcon   smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this ReportFormat.
        /// \return
        ///     The large (32x32) icon representing this ReportFormat.
        virtual QIcon   largeIcon() const = 0;

        /// \brief
        ///     Returns the preferred extension for report
        ///     files in this format.
        /// \return
        ///     The preferred extension for report files in
        ///     this format; always starts with a dot '.'.
        virtual QString preferredExtension() const = 0;

        /// \brief
        ///     Saves the report to the specified file in this format.
        /// \param report
        ///     The report to save to a file.
        /// \param fileName
        ///     The file to save the report to; overwritten if exists.
        /// \param progressListener
        ///     The callback to use for save progress notification,
        ///     nullptr == don't notify.
        /// \exception ReportException
        ///     If a save error occurs.
        virtual void    saveReport(
                                const Report * report,
                                const QString & fileName,
                                ProgressListener progressListener = nullptr
                            ) = 0;
    };

    /// \class ReportFormatManager tt3-report/API.hpp
    /// \brief The manager of known report formats.
    class TT3_REPORT_PUBLIC ReportFormatManager final
    {
        TT3_UTILITY_CLASS(ReportFormatManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered ReportFormats.
        /// \return
        ///     The set of all registered ReportFormats.
        static auto     allReportFormats() -> ReportFormats;

        /// \brief
        ///     Registers the specified ReportFormat.
        /// \param reportFormat
        ///     The ReportFormat to register
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same ReportFormat is treated
        ///     as a "success".
        static bool     registerReportFormat(IReportFormat * reportFormat);

        /// \brief
        ///     Un-registers the specified ReportFormat.
        /// \param reportFormat
        ///     The ReportFormat to un-register
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterReportFormat(IReportFormat * reportFormat);

        /// \brief
        ///     Finds a registered ReportFormat by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered ReportFormat with the required mnemonic;
        ///     nullptr if not found.
        static auto     findReportFormat(
                                const tt3::util::Mnemonic & mnemonic
                            ) -> IReportFormat *;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };
}

//  End of tt3-report/ReportFormat.hpp
