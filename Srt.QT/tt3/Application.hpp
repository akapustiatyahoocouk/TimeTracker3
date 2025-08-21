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
        Application(int &argc, char **argv);
        virtual ~Application();

        //////////
        //  QApplication
    public:
        //  TODO document
        int         exec();

        //////////
        //  Operations
    public:
        //  True if, after "exec()", restart is required, false if not
        bool        restartRequired() const { return _restartRequired; }

        //////////
        //  Implementation
    private:
        bool        _restartRequired = false;

        //  Helpers
        void        _registerStandardComponents();
        void        _selectActiveSkin();
        void        _initialize();
        void        _cleanup();

        //////////
        //  Signal handlers
    private slots:
        void        _settingValueChanged(tt3::util::AbstractSetting * setting);
    };
}

//  End of tt3/Application.hpp
