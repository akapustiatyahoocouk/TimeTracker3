//
//  tt3-gui/PushButtonDecorations.cpp - tt3::gui::PushButtonDecorations class implementation (+specializations)
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
PushButtonDecorations::PushButtonDecorations()
{
    QColor textColor = QApplication::palette().color(QPalette::ColorRole::ButtonText);
    QColor backColor = QApplication::palette().color(QPalette::ColorRole::Button);

    _initialize(textColor, backColor,QApplication::font());
}

PushButtonDecorations::PushButtonDecorations(
        QPushButton * pushButton
    )
{
    Q_ASSERT(pushButton != nullptr);

    QColor textColor = pushButton->palette().color(QPalette::ColorRole::ButtonText);
    {
        QRegularExpression regex("QPushButton[^{]*\\{[^}]*\\s+color:\\s*([^;]+);");
        QRegularExpressionMatch match = regex.match(theCurrentTheme->css());
        if (match.hasMatch())
        {
            QString colorSpec = match.captured(1).trimmed();
            textColor = tt3::util::fromString(colorSpec, textColor);
        }
    }

    QColor backColor = pushButton->palette().color(QPalette::ColorRole::Button);
    {
        QRegularExpression regex("QPushButton[^{]*\\{[^}]*\\s+background-color:\\s*([^;]+);");
        QRegularExpressionMatch match = regex.match(theCurrentTheme->css());
        if (match.hasMatch())
        {
            QString colorSpec = match.captured(1).trimmed();
            backColor = tt3::util::fromString(colorSpec, backColor);
        }
    }

    _initialize(textColor, backColor, pushButton->font());
}

//////////
//  Operations
void PushButtonDecorations::applyTo(
        QPushButton * pushButton,
        ButtonRole buttonRole
    )
{
    Q_ASSERT(pushButton != nullptr);
    QFont font;
    QColor textColor, backColor;
    switch (buttonRole)
    {
        case ButtonRole::NormalButton:
            font = this->font;
            textColor = this->foreground;
            backColor = this->background;
            break;
        case ButtonRole::DisabledButton:
            font = this->font;
            textColor = this->disabledForeground;
            backColor = this->background;
            break;
        case ButtonRole::LiveStatusButton:
            font = this->emphasisFont;
            textColor = this->foreground;
            backColor = this->liveStatusBackground;
            break;
        case ButtonRole::ErrorButton:
        default:
            font = this->font;
            textColor = this->errorForeground;
            backColor = this->errorBackground;
            break;
    }
    QString styleSheet;
    if (QFontInfo (font).bold())
    {
        styleSheet += "font: bold;";
    }
    styleSheet += "background-color: " + tt3::util::toString(backColor) +";";
    styleSheet += "color: " + tt3::util::toString(textColor) +";";
    pushButton->setStyleSheet(styleSheet);
}

//////////
//  Implementation helpers
void PushButtonDecorations::_initialize(
        const QColor & textColor,
        const QColor & backColor,
        const QFont & baseFont
    )
{
    foreground = textColor;
    disabledForeground = ColorManager::mid(textColor, backColor);
    errorForeground = ColorManager::redder(textColor);

    background = backColor;
    errorBackground = ColorManager::slightlyRedder(backColor);
    liveStatusBackground = ColorManager::slightlyGreener(backColor);

    font = baseFont;
    emphasisFont = baseFont;
    emphasisFont.setBold(true);
}

//  End of tt3-gui/PushButtonDecorations.cpp
