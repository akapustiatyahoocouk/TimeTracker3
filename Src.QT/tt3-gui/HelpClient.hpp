//
//  tt3-gui/HelpClient.hpp - tt3 help client API
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

namespace tt3::gui
{
    /// \class HelpClient tt3-gui/API.hpp
    /// \brief The TT3 help client API.
    class TT3_GUI_PUBLIC HelpClient final
    {
        UTILITY_CLASS(HelpClient)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Shows the "Help contents".
        /// \exceptions HelpException
        ///     If an error occurs.
        static void     showContents();

        /// \brief
        ///     Shows the "Help index".
        /// \exceptions HelpException
        ///     If an error occurs.
        static void     showIndex();

        /// \brief
        ///     Shows the "Help search".
        /// \exceptions HelpException
        ///     If an error occurs.
        static void     showSearch();

        /// \brief
        ///     Shows the specified Help topic.
        /// \param topic
        ///     The help topic ID of the form "/x/y/.../z/topic[.html]".
        /// \exceptions HelpException
        ///     If an error occurs.
        static void     showTopic(const QString & topic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
        static bool     _openHelpFile(const QString & fileName);
    };
}

//  End of tt3-gui/HelpClient.hpp
