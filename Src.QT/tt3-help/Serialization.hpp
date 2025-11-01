//
//  tt3-help/Serialization.hpp - Help collection persistency
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

namespace tt3::help
{
    class TT3_HELP_PUBLIC Serializer final
    {
        UTILITY_CLASS(Serializer)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Writes the help collection to an XML file.
        /// \param helpCollection
        ///     The help collection to save.
        /// \param fileName
        ///     The name of the file to save XML to.
        /// \exception HelpException
        ///     If an error occurs.
        static void     saveToFile(
                                HelpCollection * helpCollection,
                                const QString & fileName
                            );

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3-help/Serialization.hpp
