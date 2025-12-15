//
//  tt3-gui/QuickReportView.hpp - QuickReport View widget
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
    class TT3_GUI_PUBLIC QuickReport;

    /// \class QuickReportView tt3-gui/API.hpp
    /// \brief The common base class for all "QuickReport View" widgets.
    class TT3_GUI_PUBLIC QuickReportView
        :   public QWidget
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(QuickReportView)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the new widget; nullptr == none.
        explicit QuickReportView(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~QuickReportView();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the QuickReport viewed by this widget.
        /// \return
        ///     The QuickReport viewed by this widget.
        virtual auto        quickReport(
                                ) -> IQuickReport * = 0;

        /// \brief
        ///     Returns the workspace currently viewed in this widget.
        /// \details
        ///     Starts off with the "current" workspace when the
        ///     widget is constructed.
        /// \return
        ///     The workspace currently viewed in this widget;
        ///     nullptr == none.
        auto            workspace(
                            ) const -> tt3::ws::Workspace;

        /// \brief
        ///     Sets the workspace currently viewed in this widget.
        /// \param workspace
        ///     The new workspace to be viewed in this widget;
        ///     nullptr == none.
        void            setWorkspace(
                                tt3::ws::Workspace workspace
                            );

        /// \brief
        ///     Returns the credentials used by this widget to
        ///     display the workspace.
        /// \details
        ///     Starts off with the "current" credentials when
        ///     the widget is constructed.
        /// \return
        ///     The credentials used by this widget to display
        ///     the workspace.
        auto            credentials(
                            ) const -> tt3::ws::Credentials;

        /// \brief
        ///     Sets the credentials used by this widget to
        ///     display the workspace.
        /// \details
        ///     Starts off with the "current" credentials when
        ///     the widget is constructed.
        /// \param credentials
        ///     The credentials to be used by this widget to
        ///     display the workspace.
        void            setCredentials(
                                const tt3::ws::Credentials & credentials
                            );

        /// \brief
        ///     Refreshes the content of this widget.
        virtual void    refresh() = 0;

        /// \brief
        ///     Requests that refresh() be called as soon as
        ///     practicable on this widget's event loop thread.
        void            requestRefresh();

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when this widget needs to refresh().
        void            refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;

        //////////
        //  Signal handlers
    private slots:
        void            _refreshRequested();
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_QUICK_REPORT_VIEW_DEFINED

//  End of tt3-gui/QuickReportView.hpp

