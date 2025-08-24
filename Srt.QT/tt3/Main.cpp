//
//  tt3/Main.cpp - TT3 entry point
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
#include "tt3/API.hpp"
using namespace tt3;

//////////
//  TT3 entry point
int main(int argc, char *argv[])
{
    Application a(argc, argv);

    int exitCode = a.exec();
    if (exitCode < 0)
    {   //  The following line does not work in e.g. QT Creator's
        //  debugger, but after using windeployqt on the .exe AND /DLLS
        //  the tool will bring in all dependencies from QT distrib
        QProcess::startDetached(a.arguments()[0], a.arguments().mid(1));
    }
    return exitCode;
}

//  End of tt3/Main.cpp
