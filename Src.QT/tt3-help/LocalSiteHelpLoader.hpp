//
//  tt3-help/LocalSiteHelpLoader.hpp - The help loader for a local unpacked help site
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
    class TT3_HELP_PUBLIC LocalSiteHelpLoader final
    {
        UTILITY_CLASS(LocalSiteHelpLoader)

        //////////
        //  Types
    public:
        /// \brief
        ///     The agent notified of a site analysis progress.
        using ProgressListener = std::function<void(QString currentPath)>;

        //////////
        //  Operations
    public:
        static auto     loadHelpCollection(
                                const QString siteDirectory,
                                ProgressListener progressListener
                            ) -> HelpCollection *;

        //////////
        //  Implementation
    private:
        //  Helpers
        static auto     _loadSimpleHelpCollection(
                                const QString & rootDirectory
                            ) -> SimpleHelpCollection *;
        static void     _loadTopicFromDirectory(
                                SimpleHelpTopic * topic,
                                const QString & directoty
                            );
        static void     _analyzeHtmlBytes(
                                const QByteArray & htmlBytes,
                                QString & displayName
                            );
        static void     _analyzeHtmlFile(
                                const QString & htmlFileName,
                                QString & displayName
                            );
    };
}

//  End of tt3-help/LocalSiteHelpLoader.hpp
