//
//  tt3-gui/WorkStreamManager.hpp - The Work Stream Manager widget
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
    namespace Ui { class WorkStreamManager; }

    /// \class WorkStreamManager tt3-gui/API.hpp
    /// \brief The Work Stream Manager widget.
    class TT3_GUI_PUBLIC WorkStreamManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkStreamManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit WorkStreamManager(
            QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~WorkStreamManager();

        //////////
        //  Operaions
    public:
        /// \brief
        ///     Returns the workspace currently viewed in this widget.
        /// \details
        ///     Starts off with the "current" workspace
        ///     when the widget is constructed.
        /// \return
        ///     The workspace currently viewed in this widget; nullptr == none.
        auto        workspace(
            ) const -> tt3::ws::Workspace;

        /// \brief
        ///     Sets the workspace currently viewed in this widget.
        /// \param workspace
        ///     The workspace to be viewed in this widget; nullptr == none.
        void        setWorkspace(
            tt3::ws::Workspace workspace
            );

        /// \brief
        ///     Returns the credentials used by this widget to
        ///     display the workspace.
        /// \details
        ///     Starts off with the "current" credentials
        ///     when the widget is constructed.
        /// \return
        ///     The credentials used by this widget to display
        ///     the workspace.
        auto        credentials(
            ) const -> tt3::ws::Credentials;

        /// \brief
        ///     Sets the credentials used by this widget to
        ///     display the workspace.
        /// \param credentials
        ///     The credentials to be used by this widget to
        ///     display the workspace.
        void        setCredentials(
            const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     Refreshes the content of this widget.
        void        refresh();

        /// \brief
        ///     Requests that refresh() br called as soon as
        ///     practicable on this widget's event loop thread.
        void        requestRefresh();

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when this widget needs to refresh().
        void        refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;

        //  View model
        struct _WorkspaceModelImpl;
        struct _WorkStreamModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _WorkStreamModel = std::shared_ptr<_WorkStreamModelImpl>;

        using _WorkStreamModels = QList<_WorkStreamModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _WorkStreamModels   workStreamModels;     //  ordered by text
        };

        struct _WorkStreamModelImpl
        {
            _WorkStreamModelImpl(tt3::ws::WorkStream ws) : workStream(ws) {}
            //  Properties
            tt3::ws::WorkStream workStream;     //  represented by this model
            QString             text;           //  for WorkStream tree items
            QIcon               icon;           //  for WorkStream tree items
            QFont               font;           //  for WorkStream tree items
            QBrush              brush;          //  for WorkStream tree items' text
            QString             tooltip;        //  for WorkStream tree items' text
        };

        auto        _createWorkspaceModel(
                        ) -> _WorkspaceModel;
        auto        _createWorkStreamModel(
                            tt3::ws::WorkStream workStream
                        ) -> _WorkStreamModel;
        void        _filterItems(_WorkspaceModel workspaceModel);
        void        _refreshWorkspaceTree(
                            _WorkspaceModel workspaceModel
                        );
        void        _refreshWorkStreamItem(
                            QTreeWidgetItem * workStreamItem,
                            _WorkStreamModel workStreamModel
                        );

        //  Helpers
        auto        _selectedWorkStream(
                        ) -> tt3::ws::WorkStream;
        void        _setSelectedWorkStream(
                            tt3::ws::WorkStream workStream
                        );
        void        _startListeningToWorkspaceChanges();
        void        _stopListeningToWorkspaceChanges();
        void        _clearAndDisableAllControls();

        //////////
        //  Controls
    private:
        Ui::WorkStreamManager *const    _ui;
        std::unique_ptr<QMenu>  _workStreamsTreeContextMenu;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void        _currentThemeChanged(ITheme *, ITheme *);
        void        _workStreamsTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void        _workStreamsTreeWidgetCustomContextMenuRequested(QPoint);
        void        _createWorkStreamPushButtonClicked();
        void        _modifyWorkStreamPushButtonClicked();
        void        _destroyWorkStreamPushButtonClicked();
        void        _filterLineEditTextChanged(QString);
        void        _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void        _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void        _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void        _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void        _refreshRequested();
    };
}

//  End of tt3-gui/WorkStreamManager.hpp
