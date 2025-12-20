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
        /// \brief
        ///     Ahook that can be invoked when system
        ///     shutdown is detected.
        typedef void (*ShutdownHook)(void * cbData);

        //////////
        //  Operations
    public:
        /// \brief
        ///     Registers the system shutdown hook.
        /// \param shutdownHook
        ///     The hook to call if system shutdown is detected.
        /// \param cbData
        ///     The parameter for the shutdownHook invocation.
        static void     addShutdownHook(ShutdownHook shutdownHook, void * cbData);

        /// \brief
        ///     Un-registers the system shutdown hook.
        /// \param shutdownHook
        ///     The hook to un-register.
        /// \param cbData
        ///     The parameter for the shutdownHook invocation.
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

        //  Helpers
        static _Impl *  _impl();
#ifdef Q_OS_LINUX
        static void     _sigtermHandler(int);
#endif
    };
}

//  End of tt3-util/SystemShutdownHandler.hpp
