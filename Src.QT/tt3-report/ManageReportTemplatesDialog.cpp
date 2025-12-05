//
//  tt3-report/ManageReportTemplatesDialog.cpp - tt3::report::ManageReportTemplatesDialog class implementation
//  TODO localize via Resources
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
#include "tt3-report/API.hpp"
#include "ui_ManageReportTemplatesDialog.h"
using namespace tt3::report;

//////////
//  Construction/destruction
ManageReportTemplatesDialog::ManageReportTemplatesDialog(
        QWidget * parent
    ) : QDialog(parent),
        _ui(new Ui::ManageReportTemplatesDialog)
{
    _ui->setupUi(this);

    //  Create static tree widget items
    _predefinedReportsItem = new QTreeWidgetItem();
    _customReportsItem = new QTreeWidgetItem();
    _predefinedReportsItem->setText(0, "Predefined report templates");
    _customReportsItem->setText(0, "Custom report templates");
    _ui->templatesTreeWidget->addTopLevelItem(_predefinedReportsItem);
    _ui->templatesTreeWidget->addTopLevelItem(_customReportsItem);

    //  Adjust controls
    _refresh(); //  Must populate templates tree NOW
    _ui->templatesTreeWidget->expandAll();

    //  Done
    _ui->templatesTreeWidget->setFocus();
    adjustSize();
}

ManageReportTemplatesDialog::~ManageReportTemplatesDialog()
{
    delete _ui;
}

//////////
//  Operations
void ManageReportTemplatesDialog::doModal()
{
    exec();
}

//////////
//  Implementation helpers
void ManageReportTemplatesDialog::_refresh()
{
    static const QIcon openFolderIcon(":/tt3-report/Resources/Images/Misc/OpenFolderSmall.png");
    static const QIcon closedFolderIcon(":/tt3-report/Resources/Images/Misc/ClosedFolderSmall.png");

    //  Adjust "predefined templates" folder
    QList<BasicReportTemplate*> basicReportTemplates;
    for (auto reportTemplate :
         ReportTemplateManager::allReportTemplates())
    {
        if (auto basicReportTemplate =
            dynamic_cast<BasicReportTemplate*>(reportTemplate))
        {
            basicReportTemplates.append(basicReportTemplate);
        }
    }
    std::sort(
        basicReportTemplates.begin(),
        basicReportTemplates.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });

    //  Adjust "custom templates" folder
    //  TODO

    //  Adjust folder icons
    _predefinedReportsItem->setIcon(0,
        _predefinedReportsItem->isExpanded() ? openFolderIcon : closedFolderIcon);
    _customReportsItem->setIcon(0,
        _customReportsItem->isExpanded() ? openFolderIcon : closedFolderIcon);
}

//  End of tt3-report/ManageReportTemplatesDialog.cpp
