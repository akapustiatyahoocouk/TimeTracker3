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
    //  TODO kill off QtWebView::initialize();
    Application app(argc, argv);
    //  Move the next line to Application::_initialize(),
    //  right before the _selectActiveTheme() call
    app.setStyle(QStyleFactory::create("Fusion"));    //  TODO what about Linux?
    try
    {
        return app.exec();
    }
    catch (const tt3::gui::RestartRequest &)
    {   //  The following line does not work in e.g. QT Creator's
        //  debugger, but after using windeployqt on the .exe AND .dlls`
        //  the tool will bring in all dependencies from QT distrib
        QProcess::startDetached(app.arguments()[0], app.arguments().mid(1));
        return 0;
    }
}

//  End of tt3/Main.cpp
