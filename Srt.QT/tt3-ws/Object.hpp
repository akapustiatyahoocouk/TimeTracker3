//
//  tt3-ws/Object.hpp - A generic orkspace onject
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

namespace tt3::ws
{
    //  A generic workspace object
    class TT3_WS_PUBLIC ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ObjectImpl)

        //////////
        //  Construction/destruction
    private:
        ObjectImpl();
        virtual ~ObjectImpl();
    };
}

//  End of tt3-ws/DateTime.hpp
