//
//  tt3-util/SystemShutdownHandler.hpp - System shutdows handler
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
#include "tt3-util/API.hpp"

namespace tt3::util
{
    /// \class SystemShutdownHandler tt3-util/API.hpp
    /// \brief The system shutdoen handler.
    class TT3_UTIL_PUBLIC SystemShutdownHandler final
    {
        TT3_UTILITY_CLASS(SystemShutdownHandler)

        //////////
        //  Types
    public:
        typedef void (*ShutdownHook)(void * cbData);

        //////////
        //  Operations
    public:
        static void     addShutdownHook(ShutdownHook shutdownHook, void * cbData);
        static void     removeShutdownHook(ShutdownHook shutdownHook, void * cbData);

        /// \brief
        ///     Activates the system shutdown handler.
        /// \details
        ///     Repeated activation has no effect.
        static void     activate();

        /// \brief
        ///     Deactivates the system shutdown handler.
        /// \details
        ///     Repeated deactivation has no effect.
        static void     deactivate();

        /// \brief
        ///     Checks if the system shutdown is in progress.
        /// \return
        ///     True if the system shutdown is in progress.
        static bool     isShutdownInProgress();

        //////////
        //  Implementation
    private:
        struct _Impl;
        static _Impl *  _impl();
    };
}

//  End of tt3-util/SystemShutdownHandler.hpp
