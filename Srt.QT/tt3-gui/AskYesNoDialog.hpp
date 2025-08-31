//
//  tt3-gui/AskYesNoDialog.hpp - the modal "yes/no" dialog
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

namespace tt3::gui
{
    //  The modal "zes/mp" dialog
    //  TODO kill off this class!
    class TT3_GUI_PUBLIC AskYesNoDialog final
    {
        UTILITY_CLASS(AskYesNoDialog)

        //////////
        //  tYPES
    public:
        enum class Answer { Yes, No };

        //////////
        //  Operations
    public:
        static Answer   ask(QWidget * parent, const QString & title, const QString & question);
        static Answer   ask(const QString & title, const QString & question);
    };
}

//  End of tt3-gui/AskYesNoDialog.hpp

