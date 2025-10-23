//
//  tt3-gui/MyDayManager.hpp - The "My day" manager widget
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
    namespace Ui { class MyDayManager; }

    /// \class UserManager tt3-gui/API.hpp
    /// \brief The User Manager widget.
    class TT3_GUI_PUBLIC MyDayManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MyDayManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit MyDayManager(QWidget * parent);
        virtual ~MyDayManager();

        //////////
        //  Operaions
    public:
        /// \brief
        ///     Returns the workspace currently viewed in
        ///     this widget.
        /// \details
        ///     Starts off with the "current" workspace when the
        ///     widget is constructed.
        /// \return
        ///     The workspace currently viewed in this
        ///     widget; nullptr == none.
        tt3::ws::Workspace  workspace() const;

        /// \brief
        ///     Sets the workspace currently viewed in
        ///     this widget.
        /// \param workspace
        ///     The workspace to be viewed in this widget;
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
        tt3::ws::Credentials credentials() const;

        /// \brief
        ///     Sets the credentials used by this widget to
        ///     display the workspace.
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
        ///     Requests that refresh() br called as soon as
        ///     practicable on this widget's event loop thread.
        void            requestRefresh();

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when this widget needs to refresh().
        /// \details
        ///     The widget itself always subscribes to this
        ///     signal to handle internal refresh requests; however,
        ///     the signal allows external listeners to react to
        ///     the widget's refresh requests as well.
        void            refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;
        bool                    _constructed = false;
        bool                    _refreshUnderway = false;

        //  View model
        struct _MyDayModelImpl;
        struct _ItemModelImpl;
        struct _WorkModelImpl;
        struct _EventModelImpl;
        struct _CurrentActivityModelImpl;
        struct _DateModelImpl;

        using _MyDayModel = std::shared_ptr<_MyDayModelImpl>;
        using _ItemModel = std::shared_ptr<_ItemModelImpl>;
        using _WorkModel = std::shared_ptr<_WorkModelImpl>;
        using _EventModel = std::shared_ptr<_EventModelImpl>;
        using _CurrentActivityModel = std::shared_ptr<_CurrentActivityModelImpl>;
        using _DateModel = std::shared_ptr<_DateModelImpl>;

        using _ItemModels = QList<_ItemModel>;

        struct TT3_GUI_PUBLIC _MyDayModelImpl
        {
            //  Associations
            _ItemModels         itemModels; //  youngest first, oldest last
            //  Operations
            void                clear()
            {
                itemModels.clear();
            }
        };

        struct TT3_GUI_PUBLIC _ItemModelImpl
        {
            virtual ~_ItemModelImpl() = default;

            virtual QDateTime   startedAt() const = 0;  //  LOCAL TIME!
            virtual QDateTime   finishedAt() const = 0; //  LOCAL TIME!
            virtual QString     displayName() const = 0;
            virtual QIcon       icon() const = 0;
            virtual QString     tooltip() const = 0;
            virtual bool        isEmphasized() const { return false; }
            virtual QString     toString() const = 0;
        };

        struct TT3_GUI_PUBLIC _WorkModelImpl : public _ItemModelImpl
        {
            _WorkModelImpl(
                    tt3::ws::Work work,
                    const QDateTime & startedAt,
                    const QDateTime & finishedAt,
                    const QString & displayName,
                    const QIcon & icon,
                    const QString & tooltip
                ) : _work(work),
                    _startedAt(startedAt),
                    _finishedAt(finishedAt),
                    _displayName(displayName),
                    _icon(icon),
                    _tooltip(tooltip)
            {}

            virtual QDateTime   startedAt() const override { return _startedAt; }
            virtual QDateTime   finishedAt() const override { return _finishedAt; }
            virtual QString     displayName() const override { return _displayName; }
            virtual QIcon       icon() const override { return _icon; }
            virtual QString     tooltip() const override { return _tooltip; }
            virtual QString     toString() const override
            {
                return "[" +
                       _startedAt.time().toString() +
                       ".." +
                       _finishedAt.time().toString() +
                       "] " +
                       _displayName;
            }
            tt3::ws::Work       work() const { return _work; }

        private:
            const tt3::ws::Work _work;
            const QDateTime     _startedAt;     //  LOCAL TIME!
            const QDateTime     _finishedAt;    //  LOCAL TIME!
            const QString       _displayName;
            const QIcon         _icon;
            const QString       _tooltip;
        };

        struct TT3_GUI_PUBLIC _EventModelImpl : public _ItemModelImpl
        {
            _EventModelImpl(
                    tt3::ws::Event event,
                    const QDateTime & occurredAt,
                    const QString & summary,
                    const QIcon & icon,
                    const QString & tooltip
                ) : _event(event),
                    _occurredAt(occurredAt),
                    _summary(summary),
                    _icon(icon),
                    _tooltip(tooltip)
            {}

            virtual QDateTime   startedAt() const override { return _occurredAt; }
            virtual QDateTime   finishedAt() const override { return _occurredAt; }
            virtual QString     displayName() const override { return _summary; }
            virtual QIcon       icon() const override { return _icon; }
            virtual QString     tooltip() const override { return _tooltip; }
            virtual QString     toString() const override
            {
                return "[" +
                       _occurredAt.time().toString() +
                       "] " +
                       _summary;
            }
            tt3::ws::Event      event() const { return _event; }

        private:
            const tt3::ws::Event _event;
            const QDateTime     _occurredAt;     //  LOCAL TIME!
            const QString       _summary;
            const QIcon         _icon;
            const QString       _tooltip;
        };

        struct TT3_GUI_PUBLIC _CurrentActivityModelImpl : public _ItemModelImpl
        {
            _CurrentActivityModelImpl(
                    tt3::ws::Activity activity,
                    const QDateTime & startedAt,
                    const QString & displayName,
                    const QIcon & icon,
                    const QString & tooltip
                ) : _activity(activity),
                    _startedAt(startedAt),
                    _displayName(displayName),
                    _icon(icon),
                    _tooltip(tooltip)
            {}

            virtual QDateTime   startedAt() const override { return _startedAt; }
            virtual QDateTime   finishedAt() const override { return QDateTime::currentDateTime(); }
            virtual QString     displayName() const override { return _displayName; }
            virtual QIcon       icon() const override { return _icon; }
            virtual QString     tooltip() const override { return _tooltip; }
            virtual bool        isEmphasized() const { return true; }
            virtual QString     toString() const override
            {
                qint64 secs = qMax(0, _startedAt.secsTo(QDateTime::currentDateTime()));
                char s[32];
                sprintf(s, "%d:%02d:%02d",
                        int(secs / (60 * 60)),
                        int((secs / 60) % 60),
                        int(secs % 60));
                return "[" +
                       _startedAt.time().toString() +
                       "..present, " +
                       s + "] " +
                       _displayName;
            }
        private:
            const tt3::ws::Activity _activity;
            const QDateTime     _startedAt;     //  LOCAL TIME!
            const QString       _displayName;
            const QIcon         _icon;
            const QString       _tooltip;
        };

        struct TT3_GUI_PUBLIC _DateModelImpl : public _ItemModelImpl
        {
            _DateModelImpl(const QDate & date)
                :   _date(date),
                    _occurredAt(date, QTime(23, 59, 59, 999))   //  sort in reverse
            {}

            virtual QDateTime   startedAt() const override { return _occurredAt; }
            virtual QDateTime   finishedAt() const override { return _occurredAt; }
            virtual QString     displayName() const override { return toString(); }
            virtual QIcon       icon() const override { return _icon; }
            virtual QString     tooltip() const override { return ""; }
            virtual QString     toString() const override
            {
                return "========== " + QLocale().toString(_date, QLocale::ShortFormat) + " ==========";
            }

        private:
            const QDate         _date;      //  LOCAL TIME!
            const QDateTime     _occurredAt;//  LOCAL TIME!
            const QIcon         _icon;
        };

        _MyDayModel     _myDayModel;    //  currently displayed

        _MyDayModel     _createMyDayModel();
        _WorkModel      _createWorkModel(tt3::ws::Work work);
        _EventModel     _createEventModel(tt3::ws::Event event);
        auto            _createCurrentActivityModel(
                            ) -> _CurrentActivityModel;

        void            _breakLongWorks(_MyDayModel myDayModel);
        void            _addDateIndicators(_MyDayModel myDayModel);
        void            _sortChronologically(_MyDayModel myDayModel);
        void            _breakWorksOnEvents(_MyDayModel myDayModel);

        //  Helpers
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();
        void            _applyCurrentLocale();
        void            _recreateQuickPickButtons();
        void            _recreateDynamicControls();
        void            _refreslLogList();
        int             _logDepth();
        void            _setLogDepth(int logDepth);

        //////////
        //  Controls
    private:
        Ui::MyDayManager *const _ui;
        QTimer          _refreshTimer;
        //  Dynamic controls
        QList<tt3::ws::Activity>    _quickPicksList;
        QList<QPushButton*> _quickPicksButtons; //  parallel to _quickPicksButtons

        //  Drawing resources
        PushButtonDecorations   _pushButtonDecorations;
        ListWidgetDecorations   _listWidgetDecorations;

        //  Context menu support
        tt3::ws::Object     _contextMenuObject;
        std::unique_ptr<QMenu>  _contextMenu;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _currentLocaleChanged(QLocale, QLocale);
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void            _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void            _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void            _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void            _refreshRequested();
        void            _quickPicksPushButtonClicked();
        void            _logEventPushButtonClicked();
        void            _quickPickPushButtonClicked();
        void            _filterComboBoxCurrentIndexChanged(int);
        void            _viewOptionSettingValueChanged();
        void            _refreshTimerTimeout();
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _logListWidgetCustomContextMenuRequested(QPoint);
        void            _quickPickButtonCustomContextMenuRequested(QPoint);
        void            _modifyObjectContextActionTriggered();
        void            _removeActivityFromQuickPicksContextActionTriggered();
        void            _destroyObjectContextActionTriggered();
    };
}

//  End of tt3-gui/MyDayManager.hpp
