//
//  tt3-gui/CurrentActivity.hpp - The "current" activity
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
    /// \class CurrentActivity tt3-gui/API.hpp
    /// \brief The accessor for a "current" activity pseudo-variable.
    /// \details
    ///     Only one global static instance of this class exists,
    ///     and other instances should NOT be constructed.
    ///     Can be nullptr if there is no "current" Activity.
    class TT3_GUI_PUBLIC CurrentActivity final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentActivity)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentActivity();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentActivity();
        \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Replaces the "current" Activity.
        /// \details
        ///     Any errors are logged but ignored.
        /// \param activity
        ///     The activity to make "current"; nullptr == none.
        void        operator = (const tt3::ws::Activity & activity);

        /// \brief
        ///     Returns the pointer to the "current" Activity.
        /// \return
        ///     The pointer to the "current" Activity; nullptr == none.
        tt3::ws::Activity   operator -> () const;

        /// \brief
        ///     Returns the pointer to the "current" Activity.
        /// \return
        ///     The pointer to the "current" Activity; nullptr == none.
                    operator tt3::ws::Activity() const;

        /// \brief
        ///     Checks whether the "current" Activity is set (is not nullptr).
        /// \return
        ///     True if the "current" Activityis set, else false.
        bool        operator == (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks whether the "current" Activity is set (is not nullptr).
        /// \return
        ///     False if the "current" Activityis set, else true.
        bool        operator != (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks whether the sppecified activity is "current".
        /// \param activity
        ///     The activity to check.
        /// \return
        ///     True if the sppecified activity is "current", else false.
        bool        operator == (
                            tt3::ws::Activity activity
                        ) const;

        /// \brief
        ///     Checks whether the sppecified activity is "current".
        /// \param activity
        ///     The activity to check.
        /// \return
        ///     False if the sppecified activity is "current", else true.
        bool        operator != (
                            tt3::ws::Activity activity
                        ) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the last date+time (UTC) when the
        ///     "current" activity was assigned to.
        /// \return
        ///     The last date+time (UTC) when the
        ///     "current" activity was assigned to.
        QDateTime   lastChangedAt() const;

        /// \brief
        ///     Replaces the "current" activity with
        ///     the specified activity.
        /// \details
        ///     If the "current" activity exists, it is logged
        ///     as a Work item.
        ///     If the "current" activity alredy exists and
        ///     requires a comment to be entered (as Event)
        ///     when it's fnished, this is handled internally
        ///     by asking the user to supply that comment.
        ///     Similaely, if the new activity is not nullptr
        ///     and requires a comment when started, user will
        ///     be prompted to enter this comment internally.
        ///     If the comment entry dialog is cancelled, the
        ///     "current" activity remains unchanged.
        /// \param with
        ///     The activity to set as "current"; nullptr == none.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \return
        ///     True if the "current" activity was actually
        ///     replaced, false if not,
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        bool        replaceWith(
                            tt3::ws::Activity with,
                            const tt3::ws::Credentials & credentials
                        );

        /// \brief
        ///     Replaces the "current" activity with the
        ///     specified activity, using "current" credentials
        ///     for data access.
        /// \details
        ///     If the "current" activity exists, it is logged
        ///     as a Work item.
        ///     If the "current" activity alredy exists and
        ///     requires a comment to be entered (as Event)
        ///     when it's fnished, this is handled internally
        ///     by asking the user to supply that comment.
        ///     Similaely, if the new activity is not nullptr
        ///     and requires a comment when started, user will
        ///     be prompted to enter this comment internally.
        ///     If the comment entry dialog is cancelled, the
        ///     "current" activity remains unchanged.
        /// \param with
        ///     The activity to set as "current"; nullptr == none.
        /// \return
        ///     True if the "current" activity was actually
        ///     replaced, false if not,
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        bool        replaceWith(
                            tt3::ws::Activity with
                        );

        /// \brief
        ///     Drops the "current" activity.
        /// \details
        ///     Sets the "current" activity to nullptr.
        ///     If it requires a commet-when-finished,
        ///     ignores this behaviour. Does not record
        ///     a Work unit.
        void        drop();

        //////////
        //  Signals
        //  Clients are encourated to use "queued" connections.
    signals:
        /// \brief
        ///     Emitted after the current Activity has changed.
        /// \param before
        ///     The Activity "current" before the change, nullptr == none.
        /// \param after
        ///     The Activity "current" after the change, nullptr == none.
        void        changed(
                            tt3::ws::Activity before,
                            tt3::ws::Activity after
                        );

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

#if defined(TT3_GUI_LIBRARY)
    //  Building tt3-gui
#else
    //  Building tt3-gui client
    #ifdef Q_OS_WINDOWS
        Q_DECL_IMPORT CurrentActivity theCurrentActivity;
    #else
        extern CurrentActivity theCurrentActivity;
    #endif
#endif
}

//  End of tt3-gui/CurrentActivity.hpp
