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

    _refreshReportTemplateItems(_predefinedReportsItem);
    _refreshReportTemplateItems(_customReportsItem);

    //  Adjust folder icons
    _predefinedReportsItem->setIcon(0,
        (_predefinedReportsItem->isExpanded() &&
         _predefinedReportsItem->childCount() > 0)?
            openFolderIcon :
            closedFolderIcon);
    _customReportsItem->setIcon(0,
        (_customReportsItem->isExpanded() &&
         _customReportsItem->childCount() > 0) ?
            openFolderIcon :
            closedFolderIcon);

    //  Adjust action button availability
    _ui->exportPushButton->setEnabled(
        _selectedReportTemplate() != nullptr);
    _ui->importPushButton->setEnabled(true);
    /*  TODO uncomment & use
    _ui->removePushButton->setEnabled(
        dynamic_cast<CustomReportTemplate*>(_selectedReportTemplate()) != nullptr);
    */
    _ui->removePushButton->setEnabled(false);
}

void ManageReportTemplatesDialog::_refreshReportTemplateItems(QTreeWidgetItem * parentItem)
{
    QList<IReportTemplate*> reportTemplates;
    if (parentItem == _predefinedReportsItem)
    {
        for (auto reportTemplate :
             ReportTemplateManager::allReportTemplates())
        {
            if (dynamic_cast<BasicReportTemplate*>(reportTemplate))
            {
                reportTemplates.append(reportTemplate);
            }
        }
    }
    else if (parentItem == _customReportsItem)
    {   //  TODO implement
    }
    std::sort(
        reportTemplates.begin(),
        reportTemplates.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    //  Make sure the parent item has a proper number
    //  of child (report template) items...
    while (parentItem->childCount() < reportTemplates.size())
    {   //  Too few child (ReportTemplate) items
        parentItem->addChild(new QTreeWidgetItem());
    }
    while (parentItem->childCount() > reportTemplates.size())
    {   //  Too many child (ReportTemplate) items
        delete parentItem->takeChild(
            parentItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper ReportTemplate
    for (int i = 0; i < reportTemplates.size(); i++)
    {
        auto reportTemplateItem = parentItem->child(i);
        auto reportTemplate = reportTemplates[i];
        reportTemplateItem->setText(0, reportTemplate->displayName());
        reportTemplateItem->setIcon(0, reportTemplate->smallIcon());
        reportTemplateItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(reportTemplate));
    }
}

auto ManageReportTemplatesDialog::_selectedReportTemplate() -> IReportTemplate *
{
    QTreeWidgetItem * item = _ui->templatesTreeWidget->currentItem();
    return (item != nullptr && item->parent() != nullptr) ?
                item->data(0, Qt::ItemDataRole::UserRole).value<IReportTemplate*>() :
                nullptr;
}

//////////
//  Signal handlers:
void ManageReportTemplatesDialog::_templateTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    _refresh();
}

void ManageReportTemplatesDialog::_templateTreeWidgetItemExpanded(QTreeWidgetItem*)
{
    _refresh();
}

void ManageReportTemplatesDialog::_templateTreeWidgetItemCollapsed(QTreeWidgetItem*)
{
    _refresh();
}

void ManageReportTemplatesDialog::_exportPushButtonClicked()
{
    if (auto reportTemplate = _selectedReportTemplate())
    {
        QString path =
            QFileDialog::getSaveFileName(
                this,
                "Export report template",
                /*dir =*/ QString(),
                "TT3 report templates (*.tt3-rpt);;All files (*.*)");
        if (!path.isEmpty())
        {   //  Do it!
            QFile file(path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << reportTemplate->toXmlString();
                out.flush();
                file.close();
            }
            else
            {   //  OOPS!
                tt3::gui::ErrorDialog::show(this, file.errorString());
            }
        }
    }
}

void ManageReportTemplatesDialog::_importPushButtonClicked()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void ManageReportTemplatesDialog::_removePushButtonClicked()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

//  End of tt3-report/ManageReportTemplatesDialog.cpp
