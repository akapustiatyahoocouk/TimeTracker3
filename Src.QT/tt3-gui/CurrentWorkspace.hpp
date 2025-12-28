//
//  tt3-gui/CurrentWorkspace.hpp - The "current" workspace
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
    /// \class CurrentWorkspace tt3-gui/API.hpp
    /// \brief The accessor for a "current" workspace pseudo-variable.
    /// \details
    ///     Only one global static instance of this class exists,
    ///     and other instances should NOT be constructed.
    ///     Can be nullptr if there is no "current" workspace.
    class TT3_GUI_PUBLIC CurrentWorkspace final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentWorkspace)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentWorkspace();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentWorkspace();
        \
        //////////
        //  Operators
        //  IMPORTANT: Changing the "current" workspace does
        //  NOT change its "open/closed" status.
    public:
        /// \brief
        ///     Replaces the "current" workspace.
        /// \param workspace
        ///     The workspace to make "current"; nullptr == none.
        void        operator = (
                            const tt3::ws::Workspace & workspace
                        );

        /// \brief
        ///     Returns the pointer to the "current" workspace.
        /// \return
        ///     The pointer to the "current" workspace; nullptr == none.
        tt3::ws::Workspace  operator -> () const;

        /// \brief
        ///     Returns the pointer to the "current" workspace.
        /// \return
        ///     The pointer to the "current" workspace; nullptr == none.
                    operator tt3::ws::Workspace() const;

        /// \brief
        ///     Checks whether the "current" workspace is op2.
        /// \param op2
        ///     The workspace to check the "current" workspace against.
        /// \return
        ///     True if the "current" workspace is op2, else false.
        bool        operator == (tt3::ws::Workspace op2) const;

        /// \brief
        ///     Checks whether the "current" workspace is op2.
        /// \param op2
        ///     The workspace to check the "current" workspace against.
        /// \return
        ///     False if the "current" workspace is op2, else true.
        bool        operator != (tt3::ws::Workspace op2) const;

        /// \brief
        ///     Checks whether the "current" workspace is set (is not nullptr).
        /// \return
        ///     True if the "current" workspaceis set, else false.
        bool        operator == (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks whether the "current" workspace is set (is not nullptr).
        /// \return
        ///     False if the "current" workspaceis set, else true.
        bool        operator != (nullptr_t /*null*/) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Swaps the specified Workspace with the "current" Workspace.
        /// \details
        ///     Does NOT change the "open/closed" status of either one.
        /// \param other
        ///     The "other" Workspace to swap with the "current" Workspace.
        void        swap(tt3::ws::Workspace & other);

        //////////
        //  Signals
        //  Clients are encourated to use "queued" connections.
    signals:
        /// \brief
        ///     Emitted after the current workspace has changed.
        /// \param before
        ///     The workspace "current" before the change, nullptr == none.
        /// \param after
        ///     The workspace "current" after the change, nullptr == none.
        void        changed(
                            tt3::ws::Workspace before,
                            tt3::ws::Workspace after
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
        Q_DECL_IMPORT CurrentWorkspace theCurrentWorkspace;
    #else
        extern CurrentWorkspace theCurrentWorkspace;
    #endif
#endif
}

//  End of tt3-gui/CurrentWorkspace.hpp
