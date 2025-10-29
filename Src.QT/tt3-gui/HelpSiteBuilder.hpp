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
        /// \brief
        ///     [re]builds the help site from .zipped
        ///     help sources available in exedir/Help.
        /// \details
        ///     All work is done on the hidden worker thread.
        ///     Progress is reported by emitting signals below.
        /// \return
        ///     True if the help site was rebuilt
        ///     successfully, else false.
        bool        buildHelpSite();

        //////////
        //  Signals - emitted in ths order only!
    signals:
        /// \brief
        ///     Always emitted when [re]building of the help site starts.
        void        siteBuildingStarted();

        /// \brief
        ///     Emitted to report the progress of help site building.
        void        siteBuildingProgress(QString context, QString action);

        /// \brief
        ///     Always emitted when [re]building of the help site stops.
        void        siteBuildingCompleted(bool success);

        /// \brief
        ///     Emitted after help site [re]building is
        ///     completed with an error.
        void        siteBuildingError(QString errorMessage);

        //////////
        //  Implementation
    private:
        QString         _zipFilesDirectory; //  where was the .exe launched from + "/Help"
        QString         _helpSiteDirectory; //  underneath user's temp directory

        //  Requests sent to the worker thread
        struct _ServiceRequest
        {
            virtual ~_ServiceRequest() = default;
        };

        struct _RebuildHelpRequest : public _ServiceRequest
        {
            _RebuildHelpRequest(std::atomic<bool> & cs)
                :   comletionStatus(cs) {}
            std::atomic<bool> & comletionStatus;
        };

        //  Helpers - all called on the WorkerThread
        struct _HelpSource
        {
            QString     zipFileName;    //  full path
            QDateTime   zipFileTime;    //  modification time stamp, UTC
            qint64      zipFileSize;
        };
        using _HelpSources = QList<_HelpSource>;
        _HelpSources    _detectHelpSources();
        void            _processHelpSource(const _HelpSource & helpSource);
        void            _rebuildHelpSite(_RebuildHelpRequest & request);

        //  The worker thread is where work is done and
        //  signals are emitted
        class TT3_GUI_PUBLIC _WorkerThread
            :   public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_WorkerThread)

            //////////
            //  Constants
        public:
            static inline int   WaitChunkMs = 250;

            //////////
            //  Construction/destruction
        public:
            explicit _WorkerThread(HelpSiteBuilder * helpSiteBuilder)
                :   _helpSiteBuilder(helpSiteBuilder) {}
            virtual ~_WorkerThread() = default;

            //////////
            //  QThread
        protected:
            virtual void    run() override;

            //////////
            //  Operations
        public:
            void        post(_ServiceRequest * request) { _requestQueue.enqueue(request); }
            void        requestStop() { _stopRequested = true; }

            //////////
            //  Implementation
        private:
            HelpSiteBuilder *const  _helpSiteBuilder;
            std::atomic<bool>   _stopRequested = false;
            tt3::util::BlockingQueue<_ServiceRequest*>  _requestQueue;
        };
        _WorkerThread   _workerThread;
    };
}

//  End of tt3-gui/HelpSiteBuilder.hpp
