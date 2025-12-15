//
//  tt3-gui/CurrentCredentials.hpp - The "current" credentials
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
    /// \class CurrentCredentials tt3-gui/API.hpp
    /// \brief The accessor for a "current" credentials.
    class TT3_GUI_PUBLIC CurrentCredentials final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentCredentials)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentCredentials();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentCredentials();
    \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Replaces the "current" Credentials.
        /// \param credentials
        ///     The Credentials to replace the "current" Credentials with.
        void        operator = (
                            const tt3::ws::Credentials & credentials
                        );

        /// \brief
        ///     Returns the "current" credentials.
        /// \return
        ///     The "current" credentials.
                    operator const tt3::ws::Credentials &() const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether current Credentials are "valid"
        ///     (its "login" is not empty.
        /// \return
        ///     True if "current" Credentials are valid, else false.
        bool        isValid() const;

        /// \brief
        ///     Returns the login identifier of the "current" Credentials.
        /// \return
        ///     The login identifier of the "current" Credentials
        ///     or an empty string if the "current" Credentials are invalid.
        QString     login() const;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted after the current credentials have changed.
        /// \param before
        ///     The "current" Credentials before the change.
        /// \param after
        ///     The "current" Credentials after the change.
        void        changed(
                            tt3::ws::Credentials before,
                            tt3::ws::Credentials after
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
        Q_DECL_IMPORT CurrentCredentials theCurrentCredentials;
    #else
        extern CurrentCredentials theCurrentCredentials;
    #endif
#endif
}

//  End of tt3-gui/CurrentCredentials.hpp
