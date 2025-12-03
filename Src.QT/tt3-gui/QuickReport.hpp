//
//  tt3-gui/QuickReport.hpp - Quick Reports framework
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
#pragma once
#include "tt3-gui/API.hpp"

namespace tt3::gui
{
    class TT3_GUI_PUBLIC QuickReportView;

    /// \class IQuickReport tt3-gui/API.hpp
    /// \brief
    ///     A generic "quick report", that is
    ///     displayed "live" in the TT3 UI.
    /// \details
    ///     Concrete classes implementing this
    ///     interface will typically be singletons
    ///     registered by Components during their initialization.
    class TT3_GUI_PUBLIC IQuickReport
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        IQuickReport() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~IQuickReport() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this QuickReport.
        /// \return
        ///     The mnemonic identifier of this QuickReport.
        virtual Mnemonic    mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this QuickReport.
        /// \return
        ///     The user-readable display name of this QuickReport
        ///     for the current default locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this QuickReport.
        /// \return
        ///     The short (1 line) description of this QuickReport
        ///     for the current default locale.
        virtual QString     description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this QuickReport.
        /// \return
        ///     The small (16x16) icon representing this QuickReport.
        virtual QIcon       smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this QuickReport.
        /// \return
        ///     The large (32x32) icon representing this QuickReport.
        virtual QIcon       largeIcon() const = 0;

        /// \brief
        ///     Creates a new View for this QuickReport.
        /// \param parent
        ///     The parent widget for the new View, nullptr == none.
        /// \return
        ///     The new View for this QuickReport.
        virtual auto        createView(
                                    QWidget * parent
                                ) -> QuickReportView * = 0;
    };

    /// \class QuickReportManager tt3-gui/API.hpp
    /// \brief The manager of known QuickReports.
    class TT3_GUI_PUBLIC QuickReportManager final
    {
        TT3_UTILITY_CLASS(QuickReportManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered QuickReports.
        /// \return
        ///     The set of all registered QuickReports.
        static QSet<IQuickReport*>allQuickReports();

        /// \brief
        ///     Registers the specified QuickReport.
        /// \param quickReport
        ///     The QuickReport to register
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same QuickReport is treated
        ///     as a "success".
        static bool         registerQuickReport(IQuickReport * quickReport);

        /// \brief
        ///     Finds a registered QuickReport by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered QuickReport with the required mnemonic;
        ///     nullptr if not found.
        static IQuickReport *     findQuickReport(const tt3::util::Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };

    /// \class DailyWorkQuickReport tt3-gui/API.hpp
    /// \brief The "daily work" quick report.
    class TT3_GUI_PUBLIC DailyWorkQuickReport final
        :   public virtual IQuickReport
    {
        TT3_DECLARE_SINGLETON(DailyWorkQuickReport)

        //////////
        //  IQuickReport
    public:
        virtual Mnemonic    mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QString     description() const override;
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;
        virtual auto        createView(
                                    QWidget * parent
                                ) -> QuickReportView * override;
    };
}

//  End of tt3-gui/QuickReport.hpp
