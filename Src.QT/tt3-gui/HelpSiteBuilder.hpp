//
//  tt3-gui/HelpSiteBuilder.hpp - tt3 "help site" builder
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
    /// \class HelpSiteBuilder tt3-gui/API.hpp
    /// \brief The tt3 help site "builder"
    /// \details
    ///     -   Analyzes the .zip files available in the
    ///         "Help" subdirectory of the directory where
    ///         the current process was started from.
    ///     -   Creates, in a temporary location, the "help
    ///         site" by joining all files extracted from these .zips.
    class TT3_GUI_PUBLIC HelpSiteBuilder final
        :   public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HelpSiteBuilder)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the "help site builder" agent.
        HelpSiteBuilder();

        /// \brief
        ///     The class destructor.
        ~HelpSiteBuilder();

        //////////
        //  Operations
    public:
        //  TODO document
        bool        buildHelpSite();

        //////////
        //  Signals
    signals:
        //  TODO document
        void        siteBuildingProgress(const QString & action);
        void        siteBuildingCompleted();
        void        siteBuildingError(const QString & errorMessage);

        //////////
        //  Implementation
    private:
        QString         _zipFilesDirectory; //  where was the .exe launched from + "/Help"
        QString         _helpSiteDirectory; //  underneath user's temp directory
    };
}

//  End of tt3-gui/HelpSiteBuilder.hpp
