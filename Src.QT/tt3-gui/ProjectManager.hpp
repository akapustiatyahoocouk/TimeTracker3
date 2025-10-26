//
//  tt3-gui/ProjectManager.hpp - The Project Manager widget
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
    class TT3_GUI_PUBLIC SelectProjectParentDialog;
    class TT3_GUI_PUBLIC SelectWorkloadDialog;
    class TT3_GUI_PUBLIC SelectWorkloadsDialog;

    namespace Ui { class ProjectManager; }

    /// \class ProjectManager tt3-gui/API.hpp
    /// \brief The Project Manager widget
    class TT3_GUI_PUBLIC ProjectManager final : public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ProjectManager)

        friend class SelectProjectParentDialog;
        friend class SelectWorkloadDialog;
        friend class SelectWorkloadsDialog;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for this widget; nullptr == none.
        explicit ProjectManager(
                QWidget * parentr
            );

        /// \brief
        ///     The class destructor.
        virtual ~ProjectManager();

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

        //  View model
        //  Model services are "static" because they are oiggybacked
        //  on by e.g. "select new parent for a project" dialog.
        struct _WorkspaceModelImpl;
        struct _ProjectModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _ProjectModel = std::shared_ptr<_ProjectModelImpl>;

        using _ProjectModels = QList<_ProjectModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _ProjectModels  projectModels;  //  ordered by text
        };

        struct _ProjectModelImpl
        {
            _ProjectModelImpl(tt3::ws::Project pt)
                :   project(pt) {}
            //  Properties
            tt3::ws::Project project;   //  represented by this model
            QString     text;           //  for Project tree items
            QIcon       icon;           //  for Project tree items
            QFont       font;           //  for Project tree items
            QBrush      brush;          //  for Project tree items' text
            QString     tooltip;        //  for Project tree items' text
            //  Aggregations
            _ProjectModels  childModels;  //  ordered by text
        };

        static auto     _createWorkspaceModel(
                                tt3::ws::Workspace workspace,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _WorkspaceModel;
        static auto     _createProjectModel(
                                tt3::ws::Project project,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _ProjectModel;
        static void     _removeCompletedItems(
                                _WorkspaceModel workspaceModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _removeCompletedItems(
                                _ProjectModel projectModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _filterItems(
                                _WorkspaceModel workspaceModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _filterItems(
                                _ProjectModel projectModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _refreshWorkspaceTree(
                                QTreeWidget * projectsTreeWidget,
                                _WorkspaceModel workspaceModel
                            );
        static void     _refreshProjectItem(
                                QTreeWidgetItem * projectItem,
                                _ProjectModel projectModel
                            );

        //  Helpers
        auto            _selectedProject(
                            ) -> tt3::ws::Project;
        bool            _setSelectedProject(
                                tt3::ws::Project project
                            );
        bool            _setSelectedProject(
                                QTreeWidgetItem * parentItem,
                                tt3::ws::Project project
                            );
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();
        void            _applyCurrentLocale();

        //////////
        //  Controls
    private:
        Ui::ProjectManager *const    _ui;
        std::unique_ptr<QMenu>      _projectsTreeContextMenu;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _currentLocaleChanged(QLocale, QLocale);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _projectsTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _projectsTreeWidgetCustomContextMenuRequested(QPoint);
        void            _createProjectPushButtonClicked();
        void            _modifyProjectPushButtonClicked();
        void            _destroyProjectPushButtonClicked();
        void            _completeProjectPushButtonClicked();
        void            _showCompletedCheckBoxToggled(bool);
        void            _viewOptionSettingValueChanged();
        void            _filterLineEditTextChanged(QString);
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void            _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void            _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void            _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void            _refreshRequested();
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_PROJECT_MANAGER_DEFINED

//  End of tt3-gui/ProjectManager.hpp
