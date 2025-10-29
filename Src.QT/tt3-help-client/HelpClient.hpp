//
//  tt3-help-client/HelpClient.hpp - tt3-help-client help client API
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

namespace tt3::help::client
{
    /// \class HelpClient tt3-help-client/API.hpp
    /// \brief The TT3 help client API.
    class TT3_HELP_CLIENT_PUBLIC HelpClient final
    {
        UTILITY_CLASS(HelpClient)

        //////////
        //  Operations
    public:
        //  TODO document
        //  TODO all services throw if an error occurs
        static void     showContents();
        static void     showIndex();
        static void     showSearch();
        static void     showTopic(const QString & name);
    };
}

//  End of tt3-help-client/HelpClient.hpp
