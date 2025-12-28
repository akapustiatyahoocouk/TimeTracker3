//
//  tt3/Application.hpp - TT3 application
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

namespace tt3
{
    /// \class Application API.hpp
    /// \brief The TimeTracker3 application.
    class Application final
        :   public QApplication
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Application)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the Appliction instance.
        /// \param argc
        ///     The command line argument count (as passed to main()).
        /// \param argv
        ///     The command line arguments (as passed to main()).
        Application(int & argc, char ** argv);

        /// \brief
        ///     The class destructor.
        virtual ~Application();

        //////////
        //  QGuiApplication
    public:
        /// \brief
        ///     Handles an event.
        /// \param receiver
        ///     The intended receiver for the event.
        /// \param event
        ///     The event yo deliver to the receiver.
        /// \return
        ///     True if the receiver was interested in this event, else false.
        virtual bool    notify(QObject * receiver, QEvent * event) override;

        //////////
        //  QApplication
    public:
        /// \brief
        ///     Runs the application's event loop.
        /// \return
        ///     The application exit code.
        int             exec();

        //////////
        //  Implementation
    private:
        bool            _initialized = false;

        //  Helpers
        void            _prepareForLogging();
        void            _selectActiveTheme();
        void            _selectActiveSkin();
        void            _initialize();
        void            _cleanup();

        static void     _systemShutdownHook(void * cbData);

        //////////
        //  Loggins
    private:
        inline static QString   _logFileNameBase = "tt3-!.log";

        static void     _logMessageOutput(
                                QtMsgType type,
                                const QMessageLogContext & context,
                            const QString & msg
                        );
    };
}

//  End of tt3/Application.hpp
