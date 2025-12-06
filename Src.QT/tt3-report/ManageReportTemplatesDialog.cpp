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

    //  Handle refresh requests
    connect(this,
            &ManageReportTemplatesDialog::refreshRequested,
            this,
            &ManageReportTemplatesDialog::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);
    connect(this,
            &ManageReportTemplatesDialog::previewAvailable,
            this,
            &ManageReportTemplatesDialog::_previewAvailable,
            Qt::ConnectionType::QueuedConnection);

    //  Done
    _ui->templatesTreeWidget->setFocus();
    adjustSize();
}

ManageReportTemplatesDialog::~ManageReportTemplatesDialog()
{
    //  Need to kill & delete all preview generators
    for (auto previewGenerator : _previewGenerators)
    {
        previewGenerator->terminate();
        previewGenerator->wait();
        delete previewGenerator;
    }
    //  Remove any temporary preview files created
    for (auto previewFileName : _previewFileNames)
    {
        QFile(previewFileName).remove();
    }
    //  Cleanup & done
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

    //  Display preview
    auto reportTemplate = _selectedReportTemplate();
    if (reportTemplate == nullptr)
    {
        _ui->previewGroupBox->setTitle(
            "Preview not available");
        _ui->previewTextBrowser->setText("");
    }
    else if (_previews.contains(reportTemplate))
    {
        _ui->previewGroupBox->setTitle(
            "Preview of " + reportTemplate->displayName());
        _ui->previewTextBrowser->setHtml(_previews[reportTemplate]);
    }
    else if (_previewGenerators.contains(reportTemplate) &&
             _previewGenerators[reportTemplate]->isRunning())
    {
        _ui->previewGroupBox->setTitle(
            "Preview of " + reportTemplate->displayName());
        _ui->previewTextBrowser->setText(
            "<p style=\"text-align: center;\">Generating preview...</p>");
    }
    else
    {   //  Need a new preview generator
        QTemporaryFile  previewFile;
        if (previewFile.open())
        {   //  Go!
            previewFile.close();
            _previewFileNames.insert(previewFile.fileName());
            auto previewGenerator =
                new _PreviewGenerator(this, reportTemplate, previewFile.fileName());
            _previewGenerators[reportTemplate] = previewGenerator;
            previewGenerator->start();
        }
        else
        {   //  OOPS! No go! Show error message instead.
            _previews[reportTemplate] = previewFile.errorString();
        }
        //  Refresh ASAP
        emit refreshRequested();
    }
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

void ManageReportTemplatesDialog::_refreshRequested()
{
    _refresh();
}

void ManageReportTemplatesDialog::_previewAvailable(IReportTemplate * reportTemplate, QString html)
{
    _previews[reportTemplate] = html;
}

//////////
//  ManageReportTemplatesDialog::_PreviewGenerator
ManageReportTemplatesDialog::_PreviewGenerator::_PreviewGenerator(
        ManageReportTemplatesDialog * dialog,
        IReportTemplate * reportTemplate,
        const QString & htmlFileName
    ) : _dialog(dialog),
        _reportTemplate(reportTemplate),
        _htmlFileName(htmlFileName)
{
}

ManageReportTemplatesDialog::_PreviewGenerator::~_PreviewGenerator()
{
}

void ManageReportTemplatesDialog::_PreviewGenerator::run()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    //  Generate a "preview" report
    std::unique_ptr<Report> report =
        std::make_unique<Report>(
            rr.string(RID(PreviewReport.Name)),
        _reportTemplate);
    ReportSection * section =
        report->createSection("1");

    section
        ->createParagraph(
            _reportTemplate->findParagraphStyleByName(
                IParagraphStyle::TitleStyleName))
        ->createText(rr.string(RID(PreviewReport.Name)));

    section->createTableOfContent();

    section
        ->createParagraph(
            _reportTemplate->findParagraphStyleByName(
                IParagraphStyle::Heading1StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter1)));

    ReportText * topText =
        section
            ->createParagraph()
            ->createText(rr.string(RID(PreviewReport.Paragraph1)));
    ReportAnchor * anchor =
        topText->createAnchor();

    ReportList * list =
        section->createList();
    list->createItem()
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.ListItem1)));
    list->createItem()
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.ListItem2)));
    list->createItem()
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.ListItem3)));

    section
        ->createParagraph(
            _reportTemplate->findParagraphStyleByName(
                IParagraphStyle::Heading2StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter11)));
    section
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Paragraph2)));

    section
        ->createParagraph(
            _reportTemplate->findParagraphStyleByName(
                IParagraphStyle::Heading2StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter12)));
    section
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Paragraph3)));

    section
        ->createParagraph(
            _reportTemplate->findParagraphStyleByName(
                IParagraphStyle::Heading1StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter2)));
    section
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Paragraph4)));

    ReportTable * table =
        section->createTable();
    table
        ->createCell(0, 0, 1, 1)
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell11)));
    table
        ->createCell(1, 0, 1, 1)
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell12)));
    table
        ->createCell(0, 1, 1, 1)
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell21)));
    table
        ->createCell(1, 1, 1, 1)
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell22)));

    section
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Link1)))
        ->createInternalLink(anchor);

    //  ...save as HTML...
    QString html;
    try
    {
        HtmlReportFormat::instance()->saveReport(report.get(), _htmlFileName);  //  ,ay throw!
        //  ...and load HTML as a string
        QFile file(_htmlFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            html = in.readAll();
            file.close();
        }
        else
        {
            html = _htmlFileName + ": " +  file.errorString();
        }

    }
    catch (const tt3::util::Exception & ex)
    {
        html = ex.errorMessage();
    }

    //  ...and we're done
    QThread::sleep(5);  //  TODO keep? kill?
    emit _dialog->previewAvailable(_reportTemplate, html);
    emit _dialog->refreshRequested();
}

//  End of tt3-report/ManageReportTemplatesDialog.cpp
