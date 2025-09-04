//
//  tt3-gui/WidgetDecorations.hpp - Widget decoration/styling helpers
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
    //  Decorations for a QTreeWidget
    class TT3_GUI_PUBLIC TreeWidgetDecorations
    {
        //////////
        //  Construction/destruction/assignment
    public:
        TreeWidgetDecorations();    //  pick up from app's palette
        explicit TreeWidgetDecorations(QTreeWidget * treeWidget);

        //  The default copy constructor, assignment and
        //  destructors are all OK

        //////////
        //  Properties
    public:
        QBrush      itemForeground;
        QBrush      disabledItemForeground;
        QBrush      errorItemForeground;
        QBrush      filterMatchItemForeground;
        QFont       itemFont;
        QFont       itemEmphasisFont;   //  e.g. bold

        //////////
        //  Implementation
    private:
        //  Helpers
        void        _init(const QColor & textColor, const QColor & backColor,
                          const QFont & baseFont);
    };
}

//  End of tt3-gui/WidgetDecorations.hpp
