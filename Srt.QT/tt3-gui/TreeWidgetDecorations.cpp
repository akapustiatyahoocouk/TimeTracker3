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

namespace
{
    QColor mid(const QColor & a, const QColor & b)
    {
        return QColor((a.red() + b.red()) / 2,
                      (a.green() + b.green()) / 2,
                      (a.blue() + b.blue()) / 2,
                      (a.alpha() + b.alpha()) / 2);
    }

    QColor redder(const QColor & c)
    {
        if (c.red() < 128)
        {   //  just add red
            return QColor(c.red() + 128, c.green(), c.blue(), c.alpha());
        }
        else
        {   //  max the red, reduce green and blue
            return QColor(255, qMax(0, c.green() - 128), qMax(0, c.blue()) - 128, c.alpha());
        }
    }

    QColor greener(const QColor & c)
    {
        if (c.green() < 128)
        {   //  just add hreen
            return QColor(c.red(), c.green() + 128, c.blue(), c.alpha());
        }
        else
        {   //  max the green, reduce red and blue
            return QColor(qMax(0, c.red() - 128), 255, qMax(0, c.blue()) - 128, c.alpha());
        }
    }
}

//////////
//  Construction/destruction/assignment
TreeWidgetDecorations::TreeWidgetDecorations()
{
    QColor textColor = QApplication::palette().color(QPalette::ColorRole::Text);
    qDebug() << textColor;
    QColor backColor = QApplication::palette().color(QPalette::ColorRole::Base);
    qDebug() << backColor;

    _init(textColor, backColor,QApplication::font());
}

TreeWidgetDecorations::TreeWidgetDecorations(QTreeWidget * treeWidget)
{
    Q_ASSERT(treeWidget != nullptr);

    QColor textColor = treeWidget->palette().color(QPalette::ColorRole::Text);
    qDebug() << textColor;
    {
        QRegularExpression regex("QTreeWidget[^{]*\\{[^}]*\\s+color:\\s*([^;]+);");
        QRegularExpressionMatch match = regex.match(theCurrentTheme->css());
        if (match.hasMatch())
        {
            QString colorSpec = match.captured(1).trimmed();
            textColor = tt3::util::fromString<QColor>(colorSpec, textColor);
            qDebug() << textColor;
        }
    }

    QColor backColor = treeWidget->palette().color(QPalette::ColorRole::Base);
    qDebug() << backColor;
    {
        QRegularExpression regex("QTreeWidget[^{]*\\{[^}]*\\s+background-color:\\s*([^;]+);");
        QRegularExpressionMatch match = regex.match(theCurrentTheme->css());
        if (match.hasMatch())
        {
            QString colorSpec = match.captured(1).trimmed();
            backColor = tt3::util::fromString<QColor>(colorSpec, backColor);
            qDebug() << backColor;
        }
    }

    _init(textColor, backColor, treeWidget->font());
}

//////////
//  Implementation helpers
void TreeWidgetDecorations::_init(const QColor & textColor, const QColor & backColor,
                                  const QFont & baseFont)
{
    itemForeground = QBrush(textColor);
    disabledItemForeground = mid(textColor, backColor);
    errorItemForeground = QBrush(redder(textColor));
    filterMatchItemForeground = QBrush(greener(textColor));

    itemFont = baseFont;
    qDebug() << itemFont;
    itemEmphasisFont = baseFont;
    itemEmphasisFont.setBold(true);
    qDebug() << itemEmphasisFont;
}

//  End of tt3-gui/TreeWidgetDecorations.cpp
