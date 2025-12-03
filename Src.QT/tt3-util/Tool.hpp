//
//  tt3-util/Tool.hpp - tt3 Tools framework
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

namespace tt3::util
{
    /// \class ITool tt3-util/API.hpp
    /// \brief
    ///     A generic "tool" is an agent that can be
    ///     "invoked" in oder to achieve some task.
    class TT3_UTIL_PUBLIC ITool
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        ITool() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~ITool() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this tool,
        /// \return
        ///     The mnemonic identifier of this tool,
        virtual Mnemonic    mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this tool.
        /// \return
        ///     The user-readable display name of this tool
        ///     for the current default locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this tool.
        /// \return
        ///     The short (1 line) description of this tool
        ///     for the current default locale.
        virtual QString     description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this tool.
        /// \return
        ///     The small (16x16) icon representing this tool.
        virtual QIcon       smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this tool.
        /// \return
        ///     The large (32x32) icon representing this tool.
        virtual QIcon       largeIcon() const = 0;

        /// \brief
        ///     Checks if this Tool is "enabled" (i.e. can be
        ///     used) given the current state of the application.
        /// \return
        ///     True if this Tool is currently "enabled", else false.
        virtual bool        enabled() const = 0;

        /// \brief
        ///     Runs the tool to completion (or error).
        /// \details
        ///     The methos must be called from a thread that
        ///     has a running event loop, even if the tool
        ///     uses helper threads internally.
        /// \param parent
        ///     The widget to use as parent for any e.g. popup
        ///     windows, dialogs, etc. that appear while the
        ///     Tool is running; nullptr == select automatically.
        /// \exception Exception
        ///     If an error occurs running the tool.
        virtual void        run(QWidget * parent) = 0;
    };

    /// \class ToolManager tt3-util/API.hpp
    /// \brief The manager of known Tools.
    class TT3_UTIL_PUBLIC ToolManager final
    {
        TT3_UTILITY_CLASS(ToolManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered Tools.
        /// \return
        ///     The set of all registered Tools.
        static Tools    allTools();

        /// \brief
        ///     Registers the specified Tool.
        /// \param tool
        ///     The tool to register.
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same yool is treated
        ///     as a success.
        static bool     registerTool(ITool * tool);

        /// \brief
        ///     Un-registers the specified Tool.
        /// \param tool
        ///     The tool to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterTool(ITool * tool);

        /// \brief
        ///     Finds a registered Tool by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to lok for.
        /// \return
        ///     The registered Tool with the required mnemonic;
        ///     nullptr if not found.
        static ITool *  findTool(const Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-util/Tool.hpp
