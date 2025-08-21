//
//  tt3-gui/ErrorDialog.cpp - tt3::gui::ErrorDialog class implementation
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
void ErrorDialog::show(const tt3::util::Exception & ex)
{
    QMessageBox msgBox;
    //  TODO use the same technique for all qmessagebox'es
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText(ex.errorMessage());
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show(const std::exception & ex)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText(ex.what());
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show(const QString & errorMessage)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText("Unexpected error");
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show(QWidget * parent, const tt3::util::Exception & ex)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText(ex.errorMessage());
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show(QWidget * parent, const std::exception & ex)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText(ex.what());
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show(QWidget * parent, const QString & errorMessage)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

void ErrorDialog::show(QWidget * parent)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("ERROR");
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setText("Unexpected error");
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);
    msgBox.button(QMessageBox::StandardButton::Ok)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    msgBox.exec();
}

//  End of tt3-gui/ErrorDialog.cpp
