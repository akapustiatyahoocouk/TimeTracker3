//
//  tt3-gui/BeneficiaryManager.hpp - The Beneficiary Manager widget
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
    namespace Ui { class BeneficiaryManager; }

    /// \class BeneficiaryManager tt3-gui/API.hpp
    /// \brief The Beneficiary Manager widget.
    class TT3_GUI_PUBLIC BeneficiaryManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BeneficiaryManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit BeneficiaryManager(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~BeneficiaryManager();

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
        bool                    _refreshUnderway = false;

        //  View model
        struct _WorkspaceModelImpl;
        struct _BeneficiaryModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _BeneficiaryModel = std::shared_ptr<_BeneficiaryModelImpl>;

        using _BeneficiaryModels = QList<_BeneficiaryModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _BeneficiaryModels  beneficiaryModels;     //  ordered by text
        };

        struct _BeneficiaryModelImpl
        {
            _BeneficiaryModelImpl(tt3::ws::Beneficiary b) : beneficiary(b) {}
            //  Properties
            tt3::ws::Beneficiary    beneficiary;//  represented by this model
            QString             text;           //  for Beneficiary tree items
            QIcon               icon;           //  for Beneficiary tree items
            QFont               font;           //  for Beneficiary tree items
            QBrush              brush;          //  for Beneficiary tree items' text
            QString             tooltip;        //  for Beneficiary tree items' text
        };

        auto        _createWorkspaceModel(
                        ) -> _WorkspaceModel;
        auto        _createBeneficiaryModel(
                            tt3::ws::Beneficiary beneficiary
                        ) -> _BeneficiaryModel;
        void        _filterItems(_WorkspaceModel workspaceModel);
        void        _refreshWorkspaceTree(
                            _WorkspaceModel workspaceModel
                        );
        void        _refreshBeneficiaryItem(
                            QTreeWidgetItem * beneficiaryItem,
                            _BeneficiaryModel vModel
                        );

        //  Helpers
        auto        _currentBeneficiary(
                        ) -> tt3::ws::Beneficiary;
        void        _setCurrentBeneficiary(
                            tt3::ws::Beneficiary beneficiary
                        );
        void        _startListeningToWorkspaceChanges();
        void        _stopListeningToWorkspaceChanges();
        void        _clearAndDisableAllControls();
        void        _applyCurrentLocale();

        //////////
        //  Controls
    private:
        Ui::BeneficiaryManager *const  _ui;
        std::unique_ptr<QMenu>  _beneficiariesTreeContextMenu;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void        _currentThemeChanged(ITheme *, ITheme *);
        void        _currentLocaleChanged(QLocale, QLocale);
        void        _beneficiariesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void        _beneficiariesTreeWidgetCustomContextMenuRequested(QPoint);
        void        _createBeneficiaryPushButtonClicked();
        void        _modifyBeneficiaryPushButtonClicked();
        void        _destroyBeneficiaryPushButtonClicked();
        void        _filterLineEditTextChanged(QString);
        void        _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void        _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void        _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void        _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void        _refreshRequested();
    };
}

//  End of tt3-gui/BeneficiaryManager.hpp

