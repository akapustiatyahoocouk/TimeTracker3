//
//  tt3-gui/TreeWidgetDecorations.cpp - tt3::gui::TreeWidgetDecorations class implementation (+specializations)
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
}

//////////
//  Construction/destruction/assignment
TreeWidgetDecorations::TreeWidgetDecorations()
{
    QColor textColor = QApplication::palette().color(QPalette::ColorRole::Text);
    QColor backColor = QApplication::palette().color(QPalette::ColorRole::Base);

    _initialize(textColor, backColor,QApplication::font());
}

TreeWidgetDecorations::TreeWidgetDecorations(QTreeWidget * treeWidget)
{
    Q_ASSERT(treeWidget != nullptr);

    QColor textColor = treeWidget->palette().color(QPalette::ColorRole::Text);
    {
        QRegularExpression regex("QTreeWidget[^{]*\\{[^}]*\\s+color:\\s*([^;]+);");
        QRegularExpressionMatch match = regex.match(theCurrentTheme->css());
        if (match.hasMatch())
        {
            QString colorSpec = match.captured(1).trimmed();
            textColor = tt3::util::fromString(colorSpec, textColor);
        }
    }

    QColor backColor = treeWidget->palette().color(QPalette::ColorRole::Base);
    {
        QRegularExpression regex("QTreeWidget[^{]*\\{[^}]*\\s+background-color:\\s*([^;]+);");
        QRegularExpressionMatch match = regex.match(theCurrentTheme->css());
        if (match.hasMatch())
        {
            QString colorSpec = match.captured(1).trimmed();
            backColor = tt3::util::fromString(colorSpec, backColor);
        }
    }

    _initialize(textColor, backColor, treeWidget->font());
}

//////////
//  Implementation helpers
void TreeWidgetDecorations::_initialize(
        const QColor & textColor,
        const QColor & backColor,
        const QFont & baseFont
    )
{
    itemForeground = QBrush(textColor);
    disabledItemForeground = ColorManager::mid(textColor, backColor);
    errorItemForeground = QBrush(ColorManager::redder(textColor));
    filterMatchItemForeground = QBrush(ColorManager::greener(textColor));

    itemFont = baseFont;
    itemEmphasisFont = baseFont;
    itemEmphasisFont.setBold(true);
}

//  End of tt3-gui/TreeWidgetDecorations.cpp
