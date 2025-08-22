//
//  tt3-gui/AskYesNoDialog.cpp - tt3::gui::AskYesNoDialog class implementation
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

//////////
//  Operations
AskYesNoDialog::Answer AskYesNoDialog::ask(QWidget * parent, const QString & title, const QString & question)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setIcon(QMessageBox::Icon::Question);
    msgBox.setText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::StandardButton::Yes)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.button(QMessageBox::StandardButton::No)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));
    return (msgBox.exec() == QMessageBox::StandardButton::Yes) ? Answer::Yes : Answer::No;
}

AskYesNoDialog::Answer AskYesNoDialog::ask(const QString & title, const QString & question)
{
    return ask(nullptr, title, question);
}

//  End of tt3-gui/AskYesNoDialog.cpp
