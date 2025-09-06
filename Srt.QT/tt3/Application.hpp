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
    //  The "TT3 application"
    class Application final : public QApplication
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Application)

        //////////
        //  Construction/destruction
    public:
        Application(int & argc, char ** argv);
        virtual ~Application();

        //////////
        //  QApplication
    public:
        //  Rund this application's message loop.
        int         exec();

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
        //  Helpers
        void        _registerStandardComponents();
        void        _selectActiveTheme();
        void        _selectActiveSkin();
        void        _initialize();
        void        _cleanup();
    };
}

//  End of tt3/Application.hpp
