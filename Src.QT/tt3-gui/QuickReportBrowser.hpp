//
//  tt3-gui/QuickReportBrowser.hpp - The Quick Report Browser widget
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
    namespace Ui { class QuickReportBrowser; }

    /// \class QuickReportBrowser tt3-gui/API.hpp
    /// \brief The Quick Report Browser widget
    class TT3_GUI_PUBLIC QuickReportBrowser final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(QuickReportBrowser)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for this widget; nullptr == none.
        explicit QuickReportBrowser(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~QuickReportBrowser();

        //////////
        //  Operaions
    public:
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
        void            refresh();

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
        bool                    _refreshUnderway = false;

        //  Helpers
        IQuickReport *  _selectedQuickReport();
        void            _setSelectedQuickReport(IQuickReport * quickReport);
        int             _selectedRefreshInterval(); //  0 == manual refresh only
        void            _setSelectedRefreshInterval(int seconds);   //  0 == manual refresh only
        void            _clearAndDisableAllControls();

        //////////
        //  Controls
    private:
        Ui::QuickReportBrowser *const   _ui;
        //  Dynamic controls are created at runtime
        QStackedLayout *    _quickReportPanelLayout;
        QuickReportView *   _quickReportView;   //  Never nullptr!

        QTimer              _refreshTimer;

        //////////
        //  Signal handlers
    private slots:
        void                _quickReportComboBoxCurrentIndexhanged(int);
        void                _refreshIntervalComboBoxCurrentIndexChanged(int);
        void                _refreshPushButtonClicked();
        void                _refreshTimerTimeout();
        void                _refreshRequested();
    };
}

//  End of tt3-gui/QuickReportBrowser.hpp

